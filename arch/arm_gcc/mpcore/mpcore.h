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
 *  @(#) $Id: mpcore.h 604 2012-05-01 13:06:01Z ertl-honda $
 */

/*
 *  MPCORE �Υϡ��ɥ������񸻤����
 */

#ifndef TOPPERS_MPCORE_H
#define TOPPERS_MPCORE_H

#include <sil.h>
#include <arm.h>

/* 
 *  MPCORE �Υϡ��ɥ������񸻤Υġ����¸������� 
 */ 
#include <mpcore_tool.h>

/*
 *  GIC(DIC)�Ѥ����
 */
#define GICC_BASE  (MPCORE_PMR_BASE + 0x0100)
#define GICD_BASE  (MPCORE_PMR_BASE + 0x1000)

#if __TARGET_ARCH_ARM == 6
#define GIC_PRI_LEVEL 16
#else /* __TARGET_ARCH_ARM == 7 */
#define GIC_PRI_LEVEL 32
#endif /* __TARGET_ARCH_ARM == 6 */

#if __TARGET_ARCH_ARM == 7
#define GIC_SUPPORT_SECURE
#endif /* __TARGET_ARCH_ARM == 7 */

/*
 *  ��¢�����޴�Ϣ�����
 */
#define MPCORE_TM_LR     (MPCORE_PMR_BASE + 0x0600)  /* ���ɥ쥸����           */
#define MPCORE_TM_COUNT  (MPCORE_PMR_BASE + 0x0604)  /* �����󥿡��쥸����       */
#define MPCORE_TM_CNT    (MPCORE_PMR_BASE + 0x0608)  /* ����ȥ���쥸����     */
#define MPCORE_TM_ISR    (MPCORE_PMR_BASE + 0x060C)  /* ����ߥ��ơ������쥸���� */

#define MPCORE_TM_CNT_EN        0x1
#define MPCORE_TM_CNT_AR        0x2
#define MPCORE_TM_CNT_IEN       0x4
#define MPCORE_TM_CNT_PS_OFFSET   8

#define MPCORE_TM_ISR_SCBIT    0x01

#define MPCORE_TM_IRQNO          29

#if __TARGET_ARCH_ARM == 7
/*
 * Global Timer��Ϣ�������r1�ʾ�ǥ��ݡ��ȡ�
 */
#define MPCORE_GTC_COUNT_L (MPCORE_PMR_BASE + 0x0200)  /* �����󥿡��쥸����L       */
#define MPCORE_GTC_COUNT_U (MPCORE_PMR_BASE + 0x0204)  /* �����󥿡��쥸����H       */
#define MPCORE_GTC_CNT     (MPCORE_PMR_BASE + 0x0208)  /* ����ȥ���쥸����      */
#define MPCORE_GTC_COMPV_L (MPCORE_PMR_BASE + 0x0210)  /* ����ڥ��Х�塼�쥸����L */
#define MPCORE_GTC_COMPV_U (MPCORE_PMR_BASE + 0x0214)  /* ����ڥ��Х�塼�쥸����H */

#define MPCORE_GTC_CNT_TIMENA 0x01 /* �����ޡ����͡��֥� */

#endif /* __TARGET_ARCH_ARM == 7 */

/*
 *  Snoop Control Unit
 */
#define MPCORE_SCU_CTRL    (MPCORE_PMR_BASE + 0x0000)  /* ����ȥ���쥸���� */
#define MPCORE_SCU_CONFIG  (MPCORE_PMR_BASE + 0x0004)  /* ����ե�����졼�����쥸���� */
#define MPCORE_SCU_CPUST   (MPCORE_PMR_BASE + 0x0008)  /* CPU���ơ����� */
#define MPCORE_SCU_IALL    (MPCORE_PMR_BASE + 0x000C)  /* ����Х�åɥ����� */
#define MPCORE_SCU_PM_CTRL (MPCORE_PMR_BASE + 0x0010)  /* �ѥե����ޥ󥹥�˥�����쥸���� */
#define MPCORE_SCU_PM_EVT0 (MPCORE_PMR_BASE + 0x0014)  /* ��˥������󥿥��٥��0 */
#define MPCORE_SCU_PM_EVT1 (MPCORE_PMR_BASE + 0x0018)  /* ��˥������󥿥��٥��1 */
#define MPCORE_SCU_PM_MN0  (MPCORE_PMR_BASE + 0x001C)  /* ��˥�������0 */
#define MPCORE_SCU_PM_MN1  (MPCORE_PMR_BASE + 0x0020)  /* ��˥�������1 */
#define MPCORE_SCU_PM_MN2  (MPCORE_PMR_BASE + 0x0024)  /* ��˥�������2 */
#define MPCORE_SCU_PM_MN3  (MPCORE_PMR_BASE + 0x0028)  /* ��˥�������3 */
#define MPCORE_SCU_PM_MN4  (MPCORE_PMR_BASE + 0x002C)  /* ��˥�������4 */
#define MPCORE_SCU_PM_MN5  (MPCORE_PMR_BASE + 0x0030)  /* ��˥�������5 */
#define MPCORE_SCU_PM_MN6  (MPCORE_PMR_BASE + 0x0034)  /* ��˥�������6 */
#define MPCORE_SCU_PM_MN7  (MPCORE_PMR_BASE + 0x0038)  /* ��˥�������7 */


#define MPCORE_SCU_CTRL_EN (1 << 0)   /* SCU���͡��֥� */

#define MPCORE_SCU_PM_CTRL_ENA  (1 << 0)    /* �ѥե����ޥ󥹥�˥�ͭ���� */
#define MPCORE_SCU_PM_CTRL_RST  (1 << 1)    /* �ѥե����ޥ󥹥�˥��ꥻ�å� */

#define MPCORE_SCU_PM_EVT_CYC_COUNT0_4 (31 << 0) /* �������륫����Ȼ��� */

#define MPCORE_SCU_IALL_WAYS   (0x0000ffffU)

#ifndef TOPPERS_MACRO_ONLY

/*
 *  SCU�ν����
 */
extern void scu_init(void);

#if __TARGET_ARCH_ARM == 6
/*
 * SCU�����󥿤ν������MN0����ѡ�
 */
extern void scu_counter_init(void);

/*
 * SCU�����ޤΥꥻ�å�
 */
Inline void
scu_counter_reset(void)
{
	sil_wrw_mem((void *)MPCORE_SCU_PM_CTRL,
			sil_rew_mem((void *)MPCORE_SCU_PM_CTRL) | MPCORE_SCU_PM_CTRL_RST);
}

/*
 * SCU�����ޤθ����ͤ��ɤ�
 */
Inline uint_t
scu_counter_get_current(void)
{
	return (sil_rew_mem((void *)MPCORE_SCU_PM_MN0));
}
#endif /* __TARGET_ARCH_ARM == 6 */

/*
 *  SMP�⡼�ɤ�ͭ����
 */
extern void mpcore_smp_mode_enable(void);

#endif /* TOPPERS_MACRO_ONLY */
#endif /* TOPPERS_MPCORE_H */
