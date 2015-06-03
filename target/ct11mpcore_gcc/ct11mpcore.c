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
 *  $Id: ct11mpcore.c 500 2011-08-28 08:12:42Z ertl-hiro $
 */

#include "kernel_impl.h"
#include "pdic/arm_primecell_uart/arm_primecell_uart.h"

#ifdef SYS_PUTC_USER_UART0
#define SYS_PUTC_UART_BASE UART0_BASE
#elif  defined(SYS_PUTC_USER_UART1) 
#define SYS_PUTC_UART_BASE UART1_BASE
#elif  defined(SYS_PUTC_USER_UART2) 
#define SYS_PUTC_UART_BASE UART2_BASE
#elif  defined(SYS_PUTC_USER_UART3)
#define SYS_PUTC_UART_BASE UART3_BASE
#endif /* SYS_PUTC_USER_UART3 */


/*
 * GIC CPUインタフェース関連
 */

/*
 * CPUインタフェースの初期化
 */
void
gic_cpu_init(void)
{
    /* 最低優先度に設定 */
    gic_cpu_set_priority(0xf);
    
    /* CPUインタフェースを有効に */
    sil_wrw_mem((void *)(GIC_BASE + GIC_CPUIF_CCR), 1);
}

/*
 * CPUインタフェースの終了
 */
void
gic_cpu_stop(void)
{
    /* CPUインタフェースを無効に */
    sil_wrw_mem((void *)(GIC_BASE + GIC_CPUIF_CCR), 0);    
}


/*
 * GIC DISインタフェース関連
 */

/*
 * 割込み禁止
 */
void
gic_dis_disable_int(uint8_t id)
{
    uint16_t offset_addr;
    uint16_t offset_bit;

    offset_addr = (id / 32) * 4;
    offset_bit  = id % 32;

    sil_wrw_mem((void *)(GIC_BASE + GIC_DIS_ECR + offset_addr), (1 << offset_bit));
}

/*
 * 割込み許可
 */
void
gic_dis_enable_int(uint8_t id)
{
    uint16_t offset_addr;
    uint16_t offset_bit;

    offset_addr = (id / 32) * 4;
    offset_bit  = id % 32;
    
    sil_wrw_mem((void *)(GIC_BASE + GIC_DIS_ESR + offset_addr), (1 << offset_bit));
}

/*
 * 割込みペンディングクリア
 */
void
gic_dis_clear_pending(uint8_t id)
{
    uint16_t offset_addr;
    uint16_t offset_bit;

    offset_addr = (id / 32) * 4;
    offset_bit  = id % 32;

    sil_wrw_mem((void *)(GIC_BASE + GIC_DIS_PCR + offset_addr), (1 << offset_bit));
}

/*
 * 割込みペンディングセット
 */
void
gic_dis_set_pending(uint8_t id){
    uint16_t offset_addr;
    uint16_t offset_bit;

    offset_addr = (id / 32) * 4;
    offset_bit  = id % 32;

    sil_wrw_mem((void *)(GIC_BASE + GIC_DIS_PSR + offset_addr), (1 << offset_bit));    
}


/*
 * 割込み要求のチェック
 */
bool_t
gic_dis_probe_int(uint8_t id){
    uint32_t state;
    uint16_t offset_addr;
    uint16_t offset_bit;

    offset_addr = (id / 32) * 4;
    offset_bit  = id % 32;

    state = sil_rew_mem((void *)(GIC_BASE + GIC_DIS_PSR + offset_addr));
    
    if((state & (1 << offset_bit)) == (1 << offset_bit)){
        return(true);
    }else{
        return(false);
    }
}

/*
 * 割込み設定のセット
 */
void
gic_dis_config(uint8_t id, uint8_t config){
    uint16_t offset_addr;
    uint16_t offset_bit;
    uint32_t cfgr_reg_val;

    offset_addr = (id / 16) * 4;
    offset_bit  = (id % 16) * 2;

    cfgr_reg_val  = sil_rew_mem((void *)(GIC_BASE + GIC_DIS_CFGR + offset_addr));
    cfgr_reg_val &= ~(0x03U << offset_bit);
    cfgr_reg_val |= (0x03U & config) << offset_bit;
      
    sil_wrw_mem((void *)(GIC_BASE + GIC_DIS_CFGR + offset_addr), cfgr_reg_val);
}

/*
 * 割込み優先度のセット
 * 内部表現で渡す． 
 */
void
gic_dis_set_priority(uint8_t id, int pri){
    uint16_t offset_addr;
    uint16_t offset_bit;
    uint32_t pr_reg_val;

    offset_addr = (id / 4) * 4;
    offset_bit  = (id % 4) * 8;

    pr_reg_val  = sil_rew_mem((void *)(GIC_BASE + GIC_DIS_PR + offset_addr));
    pr_reg_val &= ~(0xffU << offset_bit);
    pr_reg_val |= (0x0fU & pri) << (offset_bit + 4); 
    
    sil_wrw_mem((void *)(GIC_BASE + GIC_DIS_PR + offset_addr), pr_reg_val);
}

/*
 * DICインタフェースの初期化
 */
void
gic_dis_init(void)
{
    int i;
    
    /* DICインタフェースを無効に */
    sil_wrw_mem((void *)(GIC_BASE + GIC_DIS_CR), 0);        

    /* 割込みを全て禁止 */
    for(i = 0; i < GIC_TNUM_INT/32; i++){
        sil_wrw_mem((void *)(GIC_BASE + GIC_DIS_ECR + (4 * i)), 0xffffffff);    
    }

    /* ペンディングをクリア */
    for(i = 0; i < GIC_TNUM_INT/32; i++){
        sil_wrw_mem((void *)(GIC_BASE + GIC_DIS_PCR + (4 * i)), 0xffffffff);    
    }

    /* 優先度最低に設定  */
    for(i = 0; i < GIC_TNUM_INT/4; i++){
        sil_wrw_mem((void *)(GIC_BASE + GIC_DIS_PR + (4 * i)), 0xffffffff);    
    }

    /* ターゲット初期化 */
    for(i = 0; i < GIC_TNUM_INT/4; i++){
        sil_wrw_mem((void *)(GIC_BASE + GIC_DIS_TAR + (4 * i)), 0x01010101);
    }

    /* モード初期化(1-N Level) */
    for(i = 0; i < GIC_TNUM_INT/16; i++){
        sil_wrw_mem((void *)(GIC_BASE + GIC_DIS_CFGR + (4 * i)), 0x55555555);
    }

    /* DICインタフェースを有効に */
    sil_wrw_mem((void *)(GIC_BASE + GIC_DIS_CR), 1);       
}


/*
 * DICインタフェースの終了
 */
void
gic_dis_stop(void)
{
    /* DICインタフェースを無効に */
    sil_wrw_mem((void *)(GIC_BASE + GIC_DIS_CR), 0);    
}


/*
 *  UARTからのポーリング出力
 */
void
ct11mpcore_putc(char c){
    /* バッファが空くのを待つ */
    while((sil_rew_mem((void *)(SYS_PUTC_UART_BASE + UART_FR)) & UART_FR_TXFF)
          == UART_FR_TXFF);
    /* 書き込み */
    sil_wrw_mem((void *)(SYS_PUTC_UART_BASE + UART_DR), c);
}


/*
 * ポーリング出力のための初期化
 */
void
ct11mpcore_uart_init(void){

    char c;
    
    /* UART停止 */
    sil_wrw_mem((void *)(SYS_PUTC_UART_BASE + UART_CR), 0x00);

    /* エラーフラグをクリア */
    sil_wrw_mem((void *)(SYS_PUTC_UART_BASE + UART_RSR), 0x00);
    
    /* FIFOを空にする */
    while((sil_rew_mem((void *)(SYS_PUTC_UART_BASE + UART_FR)) & UART_FR_RXFE)
          != UART_FR_RXFE){
        /* バッファからの読み込み */
        c = sil_rew_mem((void *)(SYS_PUTC_UART_BASE + UART_DR));
    }

    /* ボーレートを設定 */
    sil_wrw_mem((void *)(SYS_PUTC_UART_BASE + UART_IBRD), UART_IBRD_38400);
    sil_wrw_mem((void *)(SYS_PUTC_UART_BASE + UART_FBRD), UART_FBRD_38400);
    
    
    /* データフォーマットと，FIFOのモードを設定 */
    sil_wrw_mem((void *)(SYS_PUTC_UART_BASE + UART_LCR_H), UART_LCR_H_WLEN_8);
        
    /* UART再開 */
    sil_wrw_mem((void *)(SYS_PUTC_UART_BASE + UART_CR),
                (sil_rew_mem((void *)(SYS_PUTC_UART_BASE + UART_CR))
                 | UART_CR_RXE | UART_CR_TXE | UART_CR_UARTEN));
}
