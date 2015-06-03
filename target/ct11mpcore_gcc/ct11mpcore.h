/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2006-2011 by Embedded and Real-Time Systems Laboratory
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
 *  $Id: ct11mpcore.h 778 2012-09-08 04:55:24Z ertl-hiro $
 */

/*
 * ARM CT11MPCore with the RealView Emulation Board用ドライバ定義
 */

#ifndef TOPPERS_CT11MPCORE_EB_H
#define TOPPERS_CT11MPCORE_EB_H

#include <sil.h>

/*
 * 割込み待ち命令
 */
#define ASM_TARGET_WAIT_INTERRUPT nop

/*
 *  割込みの数
 */
#define DIC_TMIN_INTNO     0U
#define DIC_TMAX_INTNO    63U
#define DIC_TNUM_INT      64U

/*
 *  MPCore Private Memory Regin Base Address
 */
#define MPCORE_PMR_BASE  0x1f000000

/*
 *  MPCore内蔵のタイマを1MHzで動作させるためのプリスケーラーの設定値
 */
#define MPCORE_TM_PS_1MHZ    99   /* コアクロック200Mhz */

/*
 * ベースアドレス
 */
#define SYS_BASE   0x10000000

/*
 * EBボードのリソース
 */
#define EB_LED            (SYS_BASE + 0x0008)

#define EB_SYS_LOCK       (SYS_BASE + 0x0020)
#define EB_SYS_PLD_CTRL1  (SYS_BASE + 0x0074)
#define EB_SYS_PLD_CTRL2  (SYS_BASE + 0x0078)

#define EB_SYS_LOCK_LOCK   0x0000
#define EB_SYS_LOCK_UNLOCK 0xA05F

#define EB_SYS_PLD_CTRL1_INTMODE_MASK       0x01C00000
#define EB_SYS_PLD_CTRL1_INTMODE_LEGACY     0x00000000
#define EB_SYS_PLD_CTRL1_INTMODE_NEW_DCC    0x00400000
#define EB_SYS_PLD_CTRL1_INTMODE_NEW_NODCC  0x00800000
#define EB_SYS_PLD_CTRL1_INTMODE_EN_FIQ     0x01000000

/*
 * 割込み番号
 */
#define DIC_IRQNO_UART0   36
#define DIC_IRQNO_UART1   37
#define DIC_IRQNO_UART2   44  /* DICに直結 */
#define DIC_IRQNO_UART3   45  /* DICに直結 */

#ifndef TOPPERS_MACRO_ONLY
/*
 * 割込みモードの初期化
 */
Inline void
eb_init_int_mode(void)
{
    /* アンロック */
    sil_wrw_mem((void *)EB_SYS_LOCK, EB_SYS_LOCK_UNLOCK);
    sil_wrw_mem((void *)EB_SYS_PLD_CTRL1,
                (sil_rew_mem((void *)EB_SYS_PLD_CTRL1)
                 & (~EB_SYS_PLD_CTRL1_INTMODE_MASK))
                   | (EB_SYS_PLD_CTRL1_INTMODE_NEW_NODCC));
    /* ロック */
    sil_wrw_mem((void *)EB_SYS_LOCK, EB_SYS_LOCK_LOCK);
}
#endif /* TOPPERS_MACRO_ONLY */

/*
 * 内蔵タイマ関連
 */
#define TIMER01_BASE       (SYS_BASE + 0x11000)

#define TIMER01_0_LOAD     (TIMER01_BASE + 0x00)
#define TIMER01_0_VALUE    (TIMER01_BASE + 0x04)
#define TIMER01_0_CTRL     (TIMER01_BASE + 0x08)
#define TIMER01_0_CLR      (TIMER01_BASE + 0x0C)

#define TIMER_CTRL_TIMER_EN     0x00000080
#define TIMER_CTRL_PERIODIC     0x00000040
#define TIMER_CTRL_INT_EN       0x00000020
#define TIMER_CTRL_PRESCALE_1   0x00000000
#define TIMER_CTRL_PRESCALE_16  0x00000004
#define TIMER_CTRL_PRESCALE_256 0x00000008
#define TIMER_CTRL_TIMESIZE_16  0x00000000
#define TIMER_CTRL_TIMESIZE_32  0x00000002
#define TIMER_CTRL_ONE_SHOT     0x00000001

/*
 * システムコントローラ
 */
#define SYSTEM_CONTROLER (SYS_BASE + 0x1000)

#define SYSTEM_CONTROLER_TIMER01_1MHZ   0x00008000 /* 1MHzでカウント */


/*
 * UART関連の定義
 * レジスタの定義は arm_primecell_uart.h で行っている
 */
/* 
 * ベースアドレス
 */
#define UART0_BASE  (SYS_BASE + 0x9000)
#define UART1_BASE  (SYS_BASE + 0xA000)
#define UART2_BASE  (SYS_BASE + 0xB000)
#define UART3_BASE  (SYS_BASE + 0xC000)

/*
 *  BPS設定(38400bps)
 */ 
#define UART_IBRD_38400   0x27
#define UART_FBRD_38400   0x04

#ifndef TOPPERS_MACRO_ONLY
/*
 *  UARTからのポーリング出力
 */
extern void ct11mpcore_putc(char c);

/*
 * ポーリング出力のための初期化
 */
extern void ct11mpcore_uart_init(void);
#endif /* TOPPERS_MACRO_ONLY */

/*
 *  GICの設定
 */
#define GIC1_BASE (SYS_BASE + 0x40000)
#define GIC2_BASE (SYS_BASE + 0x50000)

/*
 * CORE0 の FIQへはGIC2が接続されている
 */
#define GIC_BASE  GIC2_BASE

#define GIC_CPUIF_CCR   0x0000 /* CPUコントロールレジスタ    */
#define GIC_CPUIF_PR    0x0004 /* 優先度レジスタ             */
#define GIC_CPUIF_BPR   0x0008 /* バイナリポイントレジスタ   */
#define GIC_CPUIF_ACK   0x000C /* アクノリッジレジスタ       */
#define GIC_CPUIF_EOI   0x0010 /* End of Interrupt レジスタ  */
#define GIC_CPUIF_RPR   0x0014 /* Runnning priority レジスタ */
#define GIC_CPUIF_PER   0x0016 /* Pendingレジスタ */

#define GIC_DIS_CR      0x1000 /* コントロールレジスタ   */
#define GIC_DIS_ESR     0x1100 /* 割込みイネーブルセット */
#define GIC_DIS_ECR     0x1180 /* 割込みイネーブルクリア */
#define GIC_DIS_PSR     0x1200 /* ペンディングセット     */
#define GIC_DIS_PCR     0x1280 /* ペンディングクリア     */
#define GIC_DIS_ACR     0x1300 /* アクティブレジスタ     */
#define GIC_DIS_PR      0x1400 /* 優先度レジスタ         */
#define GIC_DIS_TAR     0x1800 /* ターゲットレジスタ     */
#define GIC_DIS_CFGR    0x1C00 /* コンフィギュレーションレジスタ */
#define GIC_DIS_SFT     0x1F00 /* ソフトウェアインタラプトレジスタ */

#define GIC_CPUIF_CCR_EN      0x0001 /* 有効     */
#define GIC_DIS_CFGR_EDGE     0x03   /* エッジ割込み */
#define GIC_DIS_CFGR_LEVEL    0x01   /* レベル割込み */

#define GIC_TMIN_INTNO   0U
#define GIC_TMAX_INTNO  95U
#define GIC_TNUM_INT    96U

#define GIC_IRQNO_TIMER01   36
#define GIC_IRQNO_UART0     44
#define GIC_IRQNO_UART1     45
#define GIC_IRQNO_UART2     46
#define GIC_IRQNO_UART3     47

#ifndef TOPPERS_MACRO_ONLY
/*
 * GIC CPUインタフェース関連のドライバ
 */
/*
 * CPUの割込み優先度マスクを設定
 */ 
Inline void
gic_cpu_set_priority(int pri)
{
    sil_wrw_mem((void *)(GIC_BASE + GIC_CPUIF_PR), pri << 4);
}

/*
 * CPUの割込み優先度マスクを取得
 */ 
Inline int
gic_cpu_current_priority(void)
{
    return (sil_rew_mem((void *)(GIC_BASE + GIC_CPUIF_PR)) >> 4);    
}

/*
 * 割込みハンドラの出口で必要なIRC操作
 */
Inline void
gic_cpu_end_int(uint8_t id)
{
    sil_wrw_mem((void *)(GIC_BASE + GIC_CPUIF_EOI), id);
}

/*
 * CPUインタフェースの初期化
 */
extern void gic_cpu_init(void);

/*
 * CPUインタフェースの終了
 */
extern void gic_cpu_stop(void);

/*
 * GIC DISインタフェース関連のドライバ
 */

/*
 * 割込み禁止
 */
extern void gic_dis_disable_int(uint8_t id);

/*
 * 割込み許可
 */
extern void gic_dis_enable_int(uint8_t id);

/*
 * 割込みペンディングクリア
 */
extern void gic_dis_clear_pending(uint8_t id);

/*
 * 割込みペンディングセット
 */
extern void gic_dis_set_pending(uint8_t id);

/*
 * 割込み要求のチェック
 */
extern bool_t gic_dis_probe_int(uint8_t id);

/*
 * 割込み設定のセット
 */
extern void gic_dis_config(uint8_t id, uint8_t config);

/*
 * 割込み優先度のセット
 * 内部表現で渡す． 
 */
extern void gic_dis_set_priority(uint8_t id, int pri);

/*
 * DICインタフェースの初期化
 */
extern void gic_dis_init(void);

/*
 * DICインタフェースの終了
 */
extern void gic_dis_stop(void);

/*
 *  開発環境依存の処理(T.B.D)
 */
static void ct11mpcore_exit(void) NoReturn;

Inline void
ct11mpcore_exit(void)
{
#ifndef QEMU
	while (true) ;
#else /* QEMU*/
	/*
	 *  QEMUを終了させる
	 *
	 *  QEMUを起動時に，-semihostingオプションをつけることが必要．
	 */
	Asm("mov r0, #24\n\t"
		"svc 0x00123456");
#endif /* QEMU */
}

#endif /* TOPPERS_MACRO_ONLY */

/*
 *  Character LCD の設定
 */
#define CHARLCD_BASE       (SYS_BASE + 0x8000)

#define CHARLCD_COM        (CHARLCD_BASE + 0x000)
#define CHARLCD_DAT        (CHARLCD_BASE + 0x004)
#define CHARLCD_RD         (CHARLCD_BASE + 0x008)
#define CHARLCD_RAW        (CHARLCD_BASE + 0x00C)
#define CHARLCD_MASK       (CHARLCD_BASE + 0x010)
#define CHARLCD_STAT       (CHARLCD_BASE + 0x014)

#define CHAR_LCDRS      1<<7
#define CHAR_LCDRnW     1<<6
#define CHAR_LCDE       1<<5
#define CHAR_LCDD4      1<<4
#define CHAR_LCDD5      1<<3
#define CHAR_LCDD6      1<<2
#define CHAR_LCDD7      1<<1

#define RAMSELECT       1
#define COMMAND         0

#define CLR_DISPLAY     1
#define HOME            2
#define ENTRY_MODE_SET  4
#define DISP_CTRL       8
#define CUR_DIS_SHIFT   16
#define FUNC_SET        32
#define CGRAM_ADDR      64
#define DDRAM_ADDR      128

#define INC             2
#define DEC             0
#define COM_DIS_SHIFT   1
#define DISP_ON         4
#define CURS_ON         2
#define BLINK_ON        1
#define DISP_OFF        0
#define CURS_OFF        0
#define BLINK_OFF       0
#define DISP_SHIFT      8
#define CURS_SHIFT      0
#define SHIFT_RIGHT     4
#define SHIFT_LEFT      0
#define IF4BIT          0
#define IF8BIT          0x10
#define NUMLINES2       8
#define NUMLINES1       0
#define FONT5X10        4
#define FONT5X8         0
#define BUSYFLAG        0x80

#define CHAR_WRTIMEOUT  10          // Write time of 1mS max
#define CHAR_BSYTIMEOUT 50          // Busy time of 50mS max

#ifndef TOPPERS_MACRO_ONLY

/*
 * Character LCE の初期化
 */
extern void charlcd_init(void);

#endif /* TOPPERS_MACRO_ONLY */

#endif /* TOPPERS_CT11MPCORE_EB_H */
