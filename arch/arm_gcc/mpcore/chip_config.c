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
 *  �嵭����Ԥϡ��ʲ���(1)��(4)�ξ������������˸¤ꡤ�ܥ��եȥ���
 *  �����ܥ��եȥ���������Ѥ�����Τ�ޤࡥ�ʲ�Ʊ���ˤ���ѡ�ʣ������
 *  �ѡ������ۡʰʲ������ѤȸƤ֡ˤ��뤳�Ȥ�̵���ǵ������롥
 *  (1) �ܥ��եȥ������򥽡��������ɤη������Ѥ�����ˤϡ��嵭������
 *      ��ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ��꤬�����Τޤޤη��ǥ���
 *      ����������˴ޤޤ�Ƥ��뤳�ȡ�
 *  (2) �ܥ��եȥ������򡤥饤�֥������ʤɡ�¾�Υ��եȥ�������ȯ�˻�
 *      �ѤǤ�����Ǻ����ۤ�����ˤϡ������ۤ�ȼ���ɥ�����ȡ�����
 *      �ԥޥ˥奢��ʤɡˤˡ��嵭�����ɽ�����������Ѿ�浪��Ӳ���
 *      ��̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *  (3) �ܥ��եȥ������򡤵�����Ȥ߹���ʤɡ�¾�Υ��եȥ�������ȯ�˻�
 *      �ѤǤ��ʤ����Ǻ����ۤ�����ˤϡ����Τ����줫�ξ�����������
 *      �ȡ�
 *    (a) �����ۤ�ȼ���ɥ�����ȡ����Ѽԥޥ˥奢��ʤɡˤˡ��嵭����
 *        �ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *    (b) �����ۤη��֤��̤�������ˡ�ˤ�äơ�TOPPERS�ץ������Ȥ�
 *        ��𤹤뤳�ȡ�
 *  (4) �ܥ��եȥ����������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������뤤���ʤ�»
 *      ������⡤�嵭����Ԥ����TOPPERS�ץ������Ȥ����դ��뤳�ȡ�
 *      �ޤ����ܥ��եȥ������Υ桼���ޤ��ϥ���ɥ桼������Τ����ʤ���
 *      ͳ�˴�Ť����ᤫ��⡤�嵭����Ԥ����TOPPERS�ץ������Ȥ�
 *      ���դ��뤳�ȡ�
 * 
 *  �ܥ��եȥ������ϡ�̵�ݾڤ��󶡤���Ƥ����ΤǤ��롥�嵭����Ԥ�
 *  ���TOPPERS�ץ������Ȥϡ��ܥ��եȥ������˴ؤ��ơ�����λ�����Ū
 *  ���Ф���Ŭ������ޤ�ơ������ʤ��ݾڤ�Ԥ�ʤ����ޤ����ܥ��եȥ���
 *  �������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������������ʤ�»���˴ؤ��Ƥ⡤��
 *  ����Ǥ�����ʤ���
 * 
 *  @(#) $Id: chip_config.c 606 2012-05-02 11:17:11Z ertl-honda $
 */

/*
 * �������åȰ�¸�⥸�塼���MPCORE�ѡ�
 */

#include "kernel_impl.h"
#include <sil.h>

#ifdef TOPPERS_ENA_PERF
extern PERFCNT perf_boot_time[];
#endif /* TOPPERS_ENA_PERF */

/*
 *  ���åװ�¸�ν����
 */
void
chip_initialize(void)
{
#ifdef TOPPERS_ENA_PERF
	x_get_pcc(&perf_boot_time[0]);
	x_rst_pcc();
#endif /* TOPPERS_ENA_PERF */

	/*
	 * ARM��¸�ν����
	 */
	core_initialize();

	/*
	 * ����å����̵����
	 */
	cache_disable();

	/*
	 * MMU��ͭ����
	 */
	mmu_init();

	/*
	 * SMP�⡼�ɤ�
	 */
	mpcore_smp_mode_enable();

	/*
	 * ����å����ͭ����
	 */
	cache_enable();

#ifdef CORE0
	/*
	 * GIC �� Disributor �����
	 */
	gicd_init();
#endif

	/*
	 * DIC��CPU���󥿥ե����������
	 */
	gicc_init();

#ifdef TOPPERS_ENA_PERF
	x_get_pcc(&perf_boot_time[1]);
	x_rst_pcc();
#endif /* TOPPERS_ENA_PERF */
}

/*
 *  ���åװ�¸�ν�λ����
 */
void
chip_exit(void)
{
	extern void    software_term_hook(void);
	void (*volatile fp)(void) = software_term_hook;

	/*
	 *  software_term_hook�ؤΥݥ��󥿤򡤰�övolatile����Τ���fp����
	 *  �����Ƥ���Ȥ��Τϡ�0�Ȥ���Ӥ���Ŭ���Ǻ������ʤ��褦�ˤ��뤿
	 *  ��Ǥ��롥
	 */
	if (fp != 0) {
		(*fp)();
	}

	/*
	 *  ARM��¸�ν�λ����
	 */
	core_terminate();

	/*
	 *  GIC �� CPU Interface �����
	 */
	gicc_stop();

	/*
	 *  GIC �� Ditributor �����
	 */
	gicd_stop();
}

/*
 *  ������׵�饤���°��������
 *
 *  ASP�����ͥ�Ǥ����Ѥ����ꤷ�ơ��ѥ�᡼�����顼�ϥ����������ǥ�����
 *  �����Ƥ��롥FI4�����ͥ�����Ѥ�����ˤϡ����顼���֤��褦�ˤ��٤�
 *  �Ǥ�����
 */
void
x_config_int(INTNO intno, ATR intatr, PRI intpri)
{
	assert(VALID_INTNO(intno));
	assert(TMIN_INTPRI <= intpri && intpri <= TMAX_INTPRI);

	/*
	 *  ������׵�Υޥ���
	 *
	 *  ����ߤ�����դ����ޤޡ���٥�ȥꥬ�����å��ȥꥬ������䡤��
	 *  ����ͥ���٤������Ԥ��Τϴ��ʤ��ᡤ�����°���ˤ�����餺��
	 *  ��ö�ޥ������롥
	 */
	x_disable_int(intno);

	/*
	 * °��������
	 */
	if ((intatr & TA_EDGE) != 0U) {
		gicd_config(intno, true, true);
		x_clear_int(intno);
	}
	else {
		gicd_config(intno, false, true);
	}

	/*
	 * �����ͥ���٥ޥ���������
	 */
	gicd_set_priority(intno, INT_IPM(intpri));
	gicd_set_target(intno, 1 << x_prc_index());

	/*
	 * ����ߤ����
	 */
	if ((intatr & TA_ENAINT) != 0U){
		(void)x_enable_int(intno);
	}
}

#ifndef OMIT_DEFAULT_INT_HANDLER

/*
 *  ̤����γ���ߤ����ä����ν���
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
