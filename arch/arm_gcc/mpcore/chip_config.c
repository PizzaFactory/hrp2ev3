/*
 *  TOPPERS/HRP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      High Reliable system Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2006-2012 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 * 
 *  上記著作権者は，以下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 * 
 *  @(#) $Id: chip_config.c 606 2012-05-02 11:17:11Z ertl-honda $
 */

/*
 * ターゲット依存モジュール（MPCORE用）
 */

#include "kernel_impl.h"
#include <sil.h>

#ifdef TOPPERS_ENA_PERF
extern PERFCNT perf_boot_time[];
#endif /* TOPPERS_ENA_PERF */

/*
 *  チップ依存の初期化
 */
void
chip_initialize(void)
{
#ifdef TOPPERS_ENA_PERF
	x_get_pcc(&perf_boot_time[0]);
	x_rst_pcc();
#endif /* TOPPERS_ENA_PERF */

	/*
	 * ARM依存の初期化
	 */
	core_initialize();

	/*
	 * キャッシュを無効に
	 */
	cache_disable();

	/*
	 * MMUを有効に
	 */
	mmu_init();

	/*
	 * SMPモードへ
	 */
	mpcore_smp_mode_enable();

	/*
	 * キャッシュを有効に
	 */
	cache_enable();

#ifdef CORE0
	/*
	 * GIC の Disributor 初期化
	 */
	gicd_init();
#endif

	/*
	 * DICのCPUインタフェース初期化
	 */
	gicc_init();

#ifdef TOPPERS_ENA_PERF
	x_get_pcc(&perf_boot_time[1]);
	x_rst_pcc();
#endif /* TOPPERS_ENA_PERF */
}

/*
 *  チップ依存の終了処理
 */
void
chip_exit(void)
{
	extern void    software_term_hook(void);
	void (*volatile fp)(void) = software_term_hook;

	/*
	 *  software_term_hookへのポインタを，一旦volatile指定のあるfpに代
	 *  入してから使うのは，0との比較が最適化で削除されないようにするた
	 *  めである．
	 */
	if (fp != 0) {
		(*fp)();
	}

	/*
	 *  ARM依存の終了処理
	 */
	core_terminate();

	/*
	 *  GIC の CPU Interface を停止
	 */
	gicc_stop();

	/*
	 *  GIC の Ditributor を停止
	 */
	gicd_stop();
}

/*
 *  割込み要求ラインの属性の設定
 *
 *  ASPカーネルでの利用を想定して，パラメータエラーはアサーションでチェッ
 *  クしている．FI4カーネルに利用する場合には，エラーを返すようにすべき
 *  であろう．
 */
void
x_config_int(INTNO intno, ATR intatr, PRI intpri)
{
	assert(VALID_INTNO(intno));
	assert(TMIN_INTPRI <= intpri && intpri <= TMAX_INTPRI);

	/*
	 *  割込み要求のマスク
	 *
	 *  割込みを受け付けたまま，レベルトリガ／エッジトリガの設定や，割
	 *  込み優先度の設定を行うのは危険なため，割込み属性にかかわらず，
	 *  一旦マスクする．
	 */
	x_disable_int(intno);

	/*
	 * 属性を設定
	 */
	if ((intatr & TA_EDGE) != 0U) {
		gicd_config(intno, true, true);
		x_clear_int(intno);
	}
	else {
		gicd_config(intno, false, true);
	}

	/*
	 * 割込み優先度マスクの設定
	 */
	gicd_set_priority(intno, INT_IPM(intpri));
	gicd_set_target(intno, 1 << x_prc_index());

	/*
	 * 割込みを許可
	 */
	if ((intatr & TA_ENAINT) != 0U){
		(void)x_enable_int(intno);
	}
}

#ifndef OMIT_DEFAULT_INT_HANDLER

/*
 *  未定義の割込みが入った場合の処理
 */
void
default_int_handler(void){
	syslog_0(LOG_EMERG, "Unregistered Interrupt occurs.");
	exit_kernel();
}

#endif /* OMIT_DEFAULT_INT_HANDLER */

#ifdef TOPPERS_ENA_PERF
void
perf_boot_init(void)
{
	x_init_pcc();
	x_rst_pcc();
}
#endif /* TOPPERS_ENA_PERF */
