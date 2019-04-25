/*
 *  TOPPERS/HRP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      High Reliable system Profile Kernel
 *
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
 *  @(#) $Id: mpcore.c 604 2012-05-01 13:06:01Z ertl-honda $
 */

#include "kernel_impl.h"

/*
 *  SCU��Ϣ�δؿ�
 */

/*
 *  CP15��AUXILIARY�쥸������SA�ӥåȤ򥻥åȤ���SMP�⡼�ɤˤ���
 */
Inline void
enable_sa(void)
{
	uint32_t bits;

	CP15_AUXILIARY_READ(bits);
	bits |= CP15_AUXILIARY_SA_BIT;
	CP15_AUXILIARY_WRITE(bits);
}

Inline void
scu_enable(void)
{
	uint32_t scu_ctrl;

	/* SCU�� tag RAMS ��̵���� */
	sil_wrw_mem((void*)MPCORE_SCU_IALL, MPCORE_SCU_IALL_WAYS);

	/* SCU��ͭ���� */
	scu_ctrl  = sil_rew_mem((void *)MPCORE_SCU_CTRL);
	scu_ctrl |= MPCORE_SCU_CTRL_EN;
	sil_wrw_mem((void *)MPCORE_SCU_CTRL, scu_ctrl);
}

/*
 *  SCU�ν���������ʥޥ����ץ��å��Τ߼¹ԡ�
 */
void
scu_init(void)
{
	/* SCU ��ͭ���� */
	scu_enable();

	/* ���ƤΥ����Υ⡼�ɤ�NORMAL�Ȥ��� */
	sil_wrw_mem((void *)MPCORE_SCU_CPUST, 0x00U);
}

/*
 *  SMP�⡼�ɤ����ꤹ��
 */
void
mpcore_smp_mode_enable(void)
{
	uint32_t sr;

	/* ������߶ػ� */
	sr = current_sr();
	set_sr(sr|CPSR_IRQ_BIT|CPSR_FIQ_BIT);

	/* ���㥷���̵�� */
	dcache_clean_and_invalidate();
	icache_invalidate();

	/* Data Synchronization Barrier */
	data_sync_barrier();

	/* TLB�ν���� */
	invalidate_unfied_tlb();

	/* CP15��SMP/nAMP bit �򥻥åȤ��� */
	enable_sa();

	/* SCU ��ͭ���ˤ��� SMP�⡼�ɤ˥��åȤ��� */
	scu_enable();

	/* ����߶ػ߾��֤򸵤��᤹ */
	set_sr(sr);
}

#if __TARGET_ARCH_ARM == 6
/*
 * SCU�����󥿤ν������MN0����ѡ�
 */
void
scu_counter_init(void)
{
	/*
	 *  MN0�� Cycle Count ������
	 */
	sil_wrw_mem((void *)MPCORE_SCU_PM_EVT0,
				sil_rew_mem((void *)MPCORE_SCU_PM_EVT0) | MPCORE_SCU_PM_EVT_CYC_COUNT0_4);

	/*
	 *  �ꥻ�åȤȥ�������
	 */
	sil_wrw_mem((void *)MPCORE_SCU_PM_CTRL,
			sil_rew_mem((void *)MPCORE_SCU_PM_CTRL) | MPCORE_SCU_PM_CTRL_RST | MPCORE_SCU_PM_CTRL_ENA);
}
#endif /* __TARGET_ARCH_ARM == 6 */
