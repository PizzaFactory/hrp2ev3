/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel  
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
 *  @(#) $Id: arm.h 604 2012-05-01 13:06:01Z ertl-honda $
 */

/*
 *  ARM�Υϡ��ɥ������񸻤����
 */

#ifndef TOPPERS_ARM_H
#define TOPPERS_ARM_H

/* 
 *  ARM �Υϡ��ɥ������񸻤Υġ����¸������� 
 */ 
#include <arm_tool.h>

/*
 *  ARM �㳰�٥���
 */
#define SVC_Vector    UINT_C(0x00)
#define UND_Vector    UINT_C(0x04)
#define SWI_Vector    UINT_C(0x08)
#define PRFA_Vector   UINT_C(0x0C)
#define DATAA_Vector  UINT_C(0x10)
#define IRQ_Vector    UINT_C(0x18)
#define FIQ_Vector    UINT_C(0x1C)

/*
 * ARM �㳰�٥����ֹ�
 */
#define SVC_Number    UINT_C(0)
#define UND_Number    UINT_C(1)
#define SWI_Number    UINT_C(2)
#define PRFA_Number   UINT_C(3)
#define DATAA_Number  UINT_C(4)
#define UNNOWN_Number UINT_C(5)
#define IRQ_Number    UINT_C(6)
#define FIQ_Number    UINT_C(7)

/*
 *  CPSR ����߶ػߥӥå�
 */
#define CPSR_INT_MASK UINT_C(0xC0)
#define CPSR_IRQ_BIT  UINT_C(0x80)
#define CPSR_FIQ_BIT  UINT_C(0x40)

/*
 *  CPSR �Υ⡼�ɥӥå�
 */
#define CPSR_MODE_MASK   UINT_C(0x1f)
#define CPSR_USER        UINT_C(0x10)
#define CPSR_FIQ         UINT_C(0x11)
#define CPSR_IRQ         UINT_C(0x12)
#define CPSR_SVC         UINT_C(0x13)
#define CPSR_ABT         UINT_C(0x17)
#define CPSR_UND         UINT_C(0x1B)
#define CPSR_SYS         UINT_C(0x1F)

/*
 *  ���ץ�Υӥå����
 */
#define CP15_CONTROL_XP_BIT   (1 << 23)
#define CP15_CONTROL_V_BIT    (1 << 13)
#define CP15_CONTROL_I_BIT    (1 << 12)
#define CP15_CONTROL_C_BIT    (1 <<  2)
#define CP15_CONTROL_M_BIT    (1 <<  0)

#if __TARGET_ARCH_ARM == 6
#define CP15_AUXILIARY_SA_BIT (1 << 5)
#define CP15_CPUID_BIT        (0x0f)
#else /* __TARGET_ARCH_ARM == 7 */
#define CP15_AUXILIARY_SA_BIT (1 << 6)
#define CP15_AUXILIARY_EX_BIT (1 << 7)
#define CP15_CPUID_BIT        (0x03)
#endif /* __TARGET_ARCH_ARM == 7 */

#define CP15_TTB0_RGN_S       (1 << 1)
#define CP15_TTB0_RGN_WBWA    (1 << 3)

#if __TARGET_ARCH_ARM == 7
#define CP15_TTB0_IRGN_WBWA   ((1 << 6)|(0))
#endif /* __TARGET_ARCH_ARM == 7 */

#ifndef TOPPERS_MACRO_ONLY

/*
 *  ���ץ���Ѥ����롼����
 */
#if (__TARGET_ARCH_ARM == 6) || (__TARGET_ARCH_ARM == 7)
/*
 *  High exception vector ��Ȥ���������
 */
Inline void
set_high_vector(bool_t enable)
{
	uint32_t control;

	CP15_CONTROL_READ(control);

	if (enable) {
		control |= CP15_CONTROL_V_BIT;
	}
	else {
		control &= ~CP15_CONTROL_V_BIT;
	}

	CP15_CONTROL_WRITE(control);
}

/*
 *  �ץ��å�INDEX��0���ꥸ��ˤμ���
 */
Inline uint32_t
x_prc_index(void)
{
	uint32_t index;

	CP15_CPUID_READ(index);
	return((index & 0x0fU));
}

/*
 *  Data Synchronization Barrier
 *  �����ȯ�Ԥ��줿 read �� write �ν�λ���Ԥ�
 *  ������å��塤�֥����ץ�ǥ��������TLB�����ν�λ���Ԥ�
 *  ������̿��θ�˽񤫤줿̿��ϼ¹Ԥ���ʤ� 
 */
Inline void
data_sync_barrier(void)
{
	CP15_DATA_SYNC_BARRIER();
}

/*
 *  Data Memory Barrier
 *  ���ץ����ε��Ҥ˽��äơ���˽񤫤줿̿��ǤΥ��ꥢ��������
 *     ��λ����ޤ��Ԥġ�
 */
Inline void
data_memory_barrier(void)
{
	CP15_DATA_MEMORY_BARRIER();
}


/* 
 *  ����å����Ϣ
 */
/*
 *  TLB��̵����
 */
Inline void
invalidate_unfied_tlb(void)
{
	CP15_DATA_SYNC_BARRIER();
}

/*
 *  D����å����̵����
 */
Inline void
dcache_invalidate(void)
{
#if __TARGET_ARCH_ARM == 6
	CP15_DCACHE_INVALIDATE();
#elif __TARGET_ARCH_ARM == 7
	uint32_t bits = 0;
	uint32_t ways = 0;
	uint32_t sets = 0;

	CP15_CACHE_SIZE_SELECTION_WRITE(0);
	CP15_PBUFFER_FLUSH();
	for (ways = 0; ways < 4; ways++){
		for (sets = 0; sets < 256; sets++){
			bits = ways << 30 | sets << 5;
			CP15_DCACHE_INVALIDATE(bits);
		}
	}
#endif /* __TARGET_ARCH_ARM == 7 */
}

/*
 *  D����å���Υ��꡼���̵����
 */
Inline void
dcache_clean_and_invalidate(void)
{
#if __TARGET_ARCH_ARM == 6
	CP15_DCACHE_CLEAN_AND_INVALIDATE();
#elif __TARGET_ARCH_ARM == 7
	uint32_t bits = 0;
	uint32_t  ways = 0;
	uint32_t sets = 0;

	CP15_CACHE_SIZE_SELECTION_WRITE(0);
	CP15_PBUFFER_FLUSH();
	for (ways = 0; ways < 4; ways++){
		for (sets = 0; sets < 256; sets++){
			bits = ways << 30 | sets << 5;
			CP15_DCACHE_CLEAN_AND_INVALIDATE(bits);
		}
	}
#endif /* __TARGET_ARCH_ARM == 7 */
}

/*
 *  I����å����̵����
 */
Inline void
icache_invalidate(void)
{
	CP15_ICACHE_INVALIDATE();
}

/*
 *  �ץ�ե��å��Хåե��򥯥ꥢ
 */
Inline void
pbuffer_flash(void)
{
	CP15_PBUFFER_FLUSH();
}

/*
 *  D����å���򳫻�
 */
extern void dcache_enable(void);

/*
 *  D����å�������
 */
extern void dcache_disable(void);

/*
 *  I����å���γ���
 */
extern void icache_enable(void);

/*
 *  I����å�������
 */
extern void icache_disable(void);
	 
/*
 *  I/D����å����ξ����ͭ����
 */
extern void cache_enable(void);

/*
 *  I/D����å����ξ����̵����
 */
extern void cache_disable(void);

#endif /* (__TARGET_ARCH_ARM == 6) || (__TARGET_ARCH_ARM == 7) */

#endif  /* TOPPERS_MACRO_ONLY */

/*
 *  MMU��Ϣ�����
 */

/*
 *  ���������ȥڡ����Υ�����
 */
#define ARM_SSECTION_SIZE			UINT_C(0x1000000)
#define ARM_SECTION_SIZE			UINT_C(0x0100000)
#define ARM_LPAGE_SIZE				UINT_C(0x0010000)
#define ARM_PAGE_SIZE				UINT_C(0x0001000)

/*
 *  ���������ơ��֥�ȥڡ����ơ��֥�Υ�����
 */
#define ARM_SECTION_TABLE_SIZE		UINT_C(0x4000)
#define ARM_SECTION_TABLE_ALIGN		UINT_C(0x4000)
#define ARM_SECTION_TABLE_ENTRY		(ARM_SECTION_TABLE_SIZE / sizeof(uint32_t))

#define ARM_PAGE_TABLE_SIZE			UINT_C(0x0400)
#define ARM_PAGE_TABLE_ALIGN		UINT_C(0x0400)
#define ARM_PAGE_TABLE_ENTRY		(ARM_PAGE_TABLE_SIZE / sizeof(uint32_t))

#if __TARGET_ARCH_ARM == 5
/*
 *  ��1��٥�ǥ�������ץ��������͡�ARMv5��
 *  This also supports ARMv4 theoretically. (VMSAv4/v5)
 */
#define ARMV5_MMU_DSCR1_FAULT		0x00000U	/* �ե���� */
#define ARMV5_MMU_DSCR1_PAGETABLE	0x00001U	/* ���������ڡ����ơ��֥� */
#define ARMV5_MMU_DSCR1_SECTION		0x00002U	/* ��������� */
//#define ARMV6_MMU_DSCR1_SSECTION	0x40002U	/* �����ѡ���������� */
//
//#define ARMV6_MMU_DSCR1_NONGLOBAL	0x20000U	/* �����Х�Ǥʤ� */
//#define ARMV6_MMU_DSCR1_SHARED		0x10000U	/* �ץ��å��֤Ƕ�ͭ */
//#define ARMV6_MMU_DSCR1_APX0		0x00000U	/* APX�ӥåȤ�0 */
//#define ARMV6_MMU_DSCR1_APX1		0x08000U	/* APX�ӥåȤ�1 */
//#define ARMV6_MMU_DSCR1_TEX000		0x00000U	/* TEX�ӥåȤ�000 */
//#define ARMV6_MMU_DSCR1_TEX001		0x01000U	/* TEX�ӥåȤ�001 */
//#define ARMV6_MMU_DSCR1_TEX010		0x02000U	/* TEX�ӥåȤ�010 */
//#define ARMV6_MMU_DSCR1_TEX100		0x04000U	/* TEX�ӥåȤ�100 */
#define ARMV5_MMU_DSCR1_AP01		0x00400U	/* AP�ӥåȤ�01 */
//#define ARMV6_MMU_DSCR1_AP10		0x00800U	/* AP�ӥåȤ�10 */
//#define ARMV6_MMU_DSCR1_AP11		0x00c00U	/* AP�ӥåȤ�11 */
//#define ARMV6_MMU_DSCR1_ECC			0x00200U	/* ECC��ͭ����MPCore��*/
//#define ARMV6_MMU_DSCR1_NOEXEC		0x00010U	/* �¹��Բ� */
#define ARMV5_MMU_DSCR1_CB00		0x00000U	/* C�ӥåȤ�0��B�ӥåȤ�0 */
#define ARMV5_MMU_DSCR1_CB01		0x00004U	/* C�ӥåȤ�0��B�ӥåȤ�1 */
#define ARMV5_MMU_DSCR1_CB10		0x00008U	/* C�ӥåȤ�1��B�ӥåȤ�0 */
#define ARMV5_MMU_DSCR1_CB11		0x0000cU	/* C�ӥåȤ�1��B�ӥåȤ�1 */

/*
 *  ��2��٥�ǥ�������ץ��������͡�ARMv6��
 */
#define ARMV5_MMU_DSCR2_FAULT		0x0000U		/* �ե���� */
#define ARMV5_MMU_DSCR2_LARGE		0x0001U		/* �顼���ڡ��� */
#define ARMV5_MMU_DSCR2_SMALL		0x0002U		/* ���⡼��ڡ��� */
//
//#define ARMV6_MMU_DSCR2_NONGLOBAL	0x0800U		/* �����Х�Ǥʤ� */
//#define ARMV6_MMU_DSCR2_SHARED		0x0400U		/* �ץ��å��֤Ƕ�ͭ */
//#define ARMV6_MMU_DSCR2_APX0		0x0000U		/* APX�ӥåȤ�0 */
//#define ARMV6_MMU_DSCR2_APX1		0x0200U		/* APX�ӥåȤ�1 */
#define ARMV5_MMU_DSCR2_AP01		0x0550U		/* AP[0-4]�ӥåȤ�01 */
#define ARMV5_MMU_DSCR2_AP10		0x0AA0U		/* AP[0-4]�ӥåȤ�10 */
#define ARMV5_MMU_DSCR2_AP11		0x0FF0U		/* AP[0-4]�ӥåȤ�11 */
#define ARMV5_MMU_DSCR2_CB00		0x0000U		/* C�ӥåȤ�0��B�ӥåȤ�0 */
#define ARMV5_MMU_DSCR2_CB01		0x0004U		/* C�ӥåȤ�0��B�ӥåȤ�1 */
#define ARMV5_MMU_DSCR2_CB10		0x0008U		/* C�ӥåȤ�1��B�ӥåȤ�0 */
#define ARMV5_MMU_DSCR2_CB11		0x000cU		/* C�ӥåȤ�1��B�ӥåȤ�1 */
//
///* �顼���ڡ����Υǥ�������ץ��� */
//#define ARMV6_MMU_DSCR2L_TEX000		0x0000U		/* TEX�ӥåȤ�000 */
//#define ARMV6_MMU_DSCR2L_TEX001		0x1000U		/* TEX�ӥåȤ�001 */
//#define ARMV6_MMU_DSCR2L_TEX010		0x2000U		/* TEX�ӥåȤ�010 */
//#define ARMV6_MMU_DSCR2L_TEX100		0x4000U		/* TEX�ӥåȤ�100 */
//#define ARMV6_MMU_DSCR2L_NOEXEC		0x8000U		/* �¹��Բ� */
//
///* ���⡼��ڡ����Υǥ�������ץ��� */
//#define ARMV6_MMU_DSCR2S_TEX000		0x0000U		/* TEX�ӥåȤ�000 */
//#define ARMV6_MMU_DSCR2S_TEX001		0x0040U		/* TEX�ӥåȤ�001 */
//#define ARMV6_MMU_DSCR2S_TEX010		0x0080U		/* TEX�ӥåȤ�010 */
//#define ARMV6_MMU_DSCR2S_TEX100		0x0100U		/* TEX�ӥåȤ�100 */
//#define ARMV6_MMU_DSCR2S_NOEXEC		0x0001U		/* �¹��Բ� */
#elif __TARGET_ARCH_ARM == 6
/*
 *  ��1��٥�ǥ�������ץ��������͡�ARMv6��
 */
#define ARMV6_MMU_DSCR1_FAULT		0x00000U	/* �ե���� */
#define ARMV6_MMU_DSCR1_PAGETABLE	0x00001U	/* ���������ڡ����ơ��֥� */
#define ARMV6_MMU_DSCR1_SECTION		0x00002U	/* ��������� */
#define ARMV6_MMU_DSCR1_SSECTION	0x40002U	/* �����ѡ���������� */

#define ARMV6_MMU_DSCR1_NONGLOBAL	0x20000U	/* �����Х�Ǥʤ� */
#define ARMV6_MMU_DSCR1_SHARED		0x10000U	/* �ץ��å��֤Ƕ�ͭ */
#define ARMV6_MMU_DSCR1_APX0		0x00000U	/* APX�ӥåȤ�0 */
#define ARMV6_MMU_DSCR1_APX1		0x08000U	/* APX�ӥåȤ�1 */
#define ARMV6_MMU_DSCR1_TEX000		0x00000U	/* TEX�ӥåȤ�000 */
#define ARMV6_MMU_DSCR1_TEX001		0x01000U	/* TEX�ӥåȤ�001 */
#define ARMV6_MMU_DSCR1_TEX010		0x02000U	/* TEX�ӥåȤ�010 */
#define ARMV6_MMU_DSCR1_TEX100		0x04000U	/* TEX�ӥåȤ�100 */
#define ARMV6_MMU_DSCR1_AP01		0x00400U	/* AP�ӥåȤ�01 */
#define ARMV6_MMU_DSCR1_AP10		0x00800U	/* AP�ӥåȤ�10 */
#define ARMV6_MMU_DSCR1_AP11		0x00c00U	/* AP�ӥåȤ�11 */
#define ARMV6_MMU_DSCR1_ECC			0x00200U	/* ECC��ͭ����MPCore��*/
#define ARMV6_MMU_DSCR1_NOEXEC		0x00010U	/* �¹��Բ� */
#define ARMV6_MMU_DSCR1_CB00		0x00000U	/* C�ӥåȤ�0��B�ӥåȤ�0 */
#define ARMV6_MMU_DSCR1_CB01		0x00004U	/* C�ӥåȤ�0��B�ӥåȤ�1 */
#define ARMV6_MMU_DSCR1_CB10		0x00008U	/* C�ӥåȤ�1��B�ӥåȤ�0 */
#define ARMV6_MMU_DSCR1_CB11		0x0000cU	/* C�ӥåȤ�1��B�ӥåȤ�1 */

/*
 *  ��2��٥�ǥ�������ץ��������͡�ARMv6��
 */
#define ARMV6_MMU_DSCR2_FAULT		0x0000U		/* �ե���� */
#define ARMV6_MMU_DSCR2_LARGE		0x0001U		/* �顼���ڡ��� */
#define ARMV6_MMU_DSCR2_SMALL		0x0002U		/* ���⡼��ڡ��� */

#define ARMV6_MMU_DSCR2_NONGLOBAL	0x0800U		/* �����Х�Ǥʤ� */
#define ARMV6_MMU_DSCR2_SHARED		0x0400U		/* �ץ��å��֤Ƕ�ͭ */
#define ARMV6_MMU_DSCR2_APX0		0x0000U		/* APX�ӥåȤ�0 */
#define ARMV6_MMU_DSCR2_APX1		0x0200U		/* APX�ӥåȤ�1 */
#define ARMV6_MMU_DSCR2_AP01		0x0010U		/* AP�ӥåȤ�01 */
#define ARMV6_MMU_DSCR2_AP10		0x0020U		/* AP�ӥåȤ�10 */
#define ARMV6_MMU_DSCR2_AP11		0x0030U		/* AP�ӥåȤ�11 */
#define ARMV6_MMU_DSCR2_CB00		0x0000U		/* C�ӥåȤ�0��B�ӥåȤ�0 */
#define ARMV6_MMU_DSCR2_CB01		0x0004U		/* C�ӥåȤ�0��B�ӥåȤ�1 */
#define ARMV6_MMU_DSCR2_CB10		0x0008U		/* C�ӥåȤ�1��B�ӥåȤ�0 */
#define ARMV6_MMU_DSCR2_CB11		0x000cU		/* C�ӥåȤ�1��B�ӥåȤ�1 */

/* �顼���ڡ����Υǥ�������ץ��� */
#define ARMV6_MMU_DSCR2L_TEX000		0x0000U		/* TEX�ӥåȤ�000 */
#define ARMV6_MMU_DSCR2L_TEX001		0x1000U		/* TEX�ӥåȤ�001 */
#define ARMV6_MMU_DSCR2L_TEX010		0x2000U		/* TEX�ӥåȤ�010 */
#define ARMV6_MMU_DSCR2L_TEX100		0x4000U		/* TEX�ӥåȤ�100 */
#define ARMV6_MMU_DSCR2L_NOEXEC		0x8000U		/* �¹��Բ� */

/* ���⡼��ڡ����Υǥ�������ץ��� */
#define ARMV6_MMU_DSCR2S_TEX000		0x0000U		/* TEX�ӥåȤ�000 */
#define ARMV6_MMU_DSCR2S_TEX001		0x0040U		/* TEX�ӥåȤ�001 */
#define ARMV6_MMU_DSCR2S_TEX010		0x0080U		/* TEX�ӥåȤ�010 */
#define ARMV6_MMU_DSCR2S_TEX100		0x0100U		/* TEX�ӥåȤ�100 */
#define ARMV6_MMU_DSCR2S_NOEXEC		0x0001U		/* �¹��Բ� */
#endif /* __TARGET_ARCH_ARM == 6 */

#ifndef TOPPERS_MACRO_ONLY
/*
 *  MMU�ν����
 */
extern void mmu_init(void);
#endif  /* TOPPERS_MACRO_ONLY */

#endif /* TOPPERS_ARM_H */
