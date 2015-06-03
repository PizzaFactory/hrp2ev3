/*
 *  TOPPERS/HRP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      High Reliable system Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2004-2009 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id
 */
#ifndef TOPPERS_CORE_SVC_H
#define TOPPERS_CORE_SVC_H

#define	SERVICE_CALL_NUM	1

/*
 *  �����ͥ�Υ����ӥ�������Υ��󥿥ե�����
 *
 *  ������å��쥸����
 *    r7 : ���եȥ������㳰�ϥ�ɥ�����������¸���륹����å��쥸������
 *         ����ߤȹ�碌�뤿�����¸���ʤ����ᡤ������å��쥸�����Ȥ��롥
 *    lr : �����ͥ�ɥᥤ��Υ��������ĥSVC��ARM��SVC�⡼�ɤ�ư����뤿�ᡤ
 *         ���եȥ������㳰�ϥ�ɥ�ƤӽФ����˾�񤭤���뤿�᥹����å��쥸
 *         �����Ȥ��롥
 *         
 */

#define CAL_SVC_0(TYPE, FNCD) \
	register TYPE r0 asm("r0"); \
	FN r7 = FNCD; \
	Asm ( \
		"mov r7, %1\n\t" \
		"svc %2\n\t" \
		:"=r"(r0) \
		:"r"(r7),"I"(SERVICE_CALL_NUM) \
		:"r7","lr" \
	); \
	return(r0);

#define CAL_SVC_1(TYPE, FNCD, TYPE1, PAR1) \
	register TYPE1 r0 asm("r0") = (TYPE1)(PAR1); \
	FN r7 = FNCD; \
	Asm ( \
		"mov r7, %1\n\t" \
		"svc %2\n\t" \
		:"=r"(r0) \
		:"r"(r7),"I"(SERVICE_CALL_NUM),"0"(r0)\
		:"r7","lr" \
	); \
	return((TYPE)r0);

#define CAL_SVC_2(TYPE, FNCD, TYPE1, PAR1, TYPE2, PAR2) \
	register TYPE1 r0 asm("r0") = (TYPE1)(PAR1); \
	register TYPE2 r1 asm("r1") = (TYPE2)(PAR2); \
	FN r7 = FNCD; \
	Asm ( \
		"mov r7, %1\n\t" \
		"svc %2\n\t" \
		:"=r"(r0) \
		:"r"(r7),"I"(SERVICE_CALL_NUM),"0"(r0),"r"(r1)\
		:"r7","lr" \
	); \
	return((TYPE)r0);

#define CAL_SVC_3(TYPE, FNCD, TYPE1, PAR1, \
							TYPE2, PAR2, TYPE3, PAR3) \
	register TYPE1 r0 asm("r0") = (TYPE1)(PAR1); \
	register TYPE2 r1 asm("r1") = (TYPE2)(PAR2); \
	register TYPE3 r2 asm("r2") = (TYPE3)(PAR3); \
	FN r7 = FNCD; \
	Asm ( \
		"mov r7, %1\n\t" \
		"svc %2\n\t" \
		:"=r"(r0) \
		:"r"(r7),"I"(SERVICE_CALL_NUM),"0"(r0),"r"(r1),"r"(r2)\
		:"r7","lr" \
	); \
	return((TYPE)r0);

#define CAL_SVC_4(TYPE, FNCD, TYPE1, PAR1, TYPE2, PAR2, \
								TYPE3, PAR3, TYPE4, PAR4) \
	register TYPE1 r0 asm("r0") = (TYPE1)(PAR1); \
	register TYPE2 r1 asm("r1") = (TYPE2)(PAR2); \
	register TYPE3 r2 asm("r2") = (TYPE3)(PAR3); \
	register TYPE4 r3 asm("r3") = (TYPE4)(PAR4); \
	FN r7 = FNCD; \
	Asm ( \
		"mov r7, %1\n\t" \
		"svc %2\n\t" \
		:"=r"(r0) \
		:"r"(r7),"I"(SERVICE_CALL_NUM),"0"(r0),"r"(r1),"r"(r2),"r"(r3)\
		:"r7","lr" \
	); \
	return((TYPE)r0);

#define CAL_SVC_5(TYPE, FNCD, TYPE1, PAR1, TYPE2, PAR2, \
						TYPE3, PAR3, TYPE4, PAR4, TYPE5, PAR5) \
	register TYPE1 r0 asm("r0") = (TYPE1)(PAR1); \
	register TYPE2 r1 asm("r1") = (TYPE2)(PAR2); \
	register TYPE3 r2 asm("r2") = (TYPE3)(PAR3); \
	register TYPE4 r3 asm("r3") = (TYPE4)(PAR4); \
	register TYPE5 r4 asm("r4") = (TYPE5)(PAR5); \
	FN r7 = FNCD; \
	Asm ( \
		"mov r7, %1\n\t" \
		"svc %2\n\t" \
		:"=r"(r0) \
		:"r"(r7),"I"(SERVICE_CALL_NUM),"0"(r0),"r"(r1),"r"(r2),"r"(r3),"r"(r4)\
		:"r7","lr" \
	); \
	return((TYPE)r0);


#define CAL_SVC_0M(TYPE, FNCD)	\
	register TYPE r0 asm("r0"); \
	FN r7 = FNCD; \
	Asm ( \
		"mov r7, %1\n\t" \
		"svc %2\n\t" \
		:"=r"(r0) \
		:"r"(r7),"I"(SERVICE_CALL_NUM)\
		:"memory","r7","lr" \
	); \
	return((TYPE)r0);

#define CAL_SVC_1M(TYPE, FNCD, TYPE1, PAR1) \
	register TYPE1 r0 asm("r0") = (TYPE1)(PAR1); \
	FN r7 = FNCD; \
	Asm ( \
		"mov r7, %1\n\t" \
		"svc %2\n\t" \
		:"=r"(r0) \
		:"r"(r7),"I"(SERVICE_CALL_NUM),"0"(r0)\
		:"memory","r7","lr" \
	); \
	return((TYPE)r0);

#define CAL_SVC_2M(TYPE, FNCD, TYPE1, PAR1, TYPE2, PAR2) \
	register TYPE1 r0 asm("r0") = (TYPE1)(PAR1); \
	register TYPE2 r1 asm("r1") = (TYPE2)(PAR2); \
	FN r7 = FNCD; \
	Asm ( \
		"mov r7, %1\n\t" \
		"svc %2\n\t" \
		:"=r"(r0) \
		:"r"(r7),"I"(SERVICE_CALL_NUM),"0"(r0),"r"(r1)\
		:"memory","r7","lr" \
	); \
	return((TYPE)r0);

#define CAL_SVC_3M(TYPE, FNCD, TYPE1, PAR1, \
							TYPE2, PAR2, TYPE3, PAR3) \
	register TYPE1 r0 asm("r0") = (TYPE1)(PAR1); \
	register TYPE2 r1 asm("r1") = (TYPE2)(PAR2); \
	register TYPE3 r2 asm("r2") = (TYPE3)(PAR3); \
	FN r7 = FNCD; \
	Asm ( \
		"mov r7, %1\n\t" \
		"svc %2\n\t" \
		:"=r"(r0) \
		:"r"(r7),"I"(SERVICE_CALL_NUM),"0"(r0),"r"(r1),"r"(r2)\
		:"memory","r7","lr" \
	); \
	return((TYPE)r0);

#define CAL_SVC_4M(TYPE, FNCD, TYPE1, PAR1, TYPE2, PAR2, \
								TYPE3, PAR3, TYPE4, PAR4) \
	register TYPE1 r0 asm("r0") = (TYPE1)(PAR1); \
	register TYPE2 r1 asm("r1") = (TYPE2)(PAR2); \
	register TYPE3 r2 asm("r2") = (TYPE3)(PAR3); \
	register TYPE4 r3 asm("r3") = (TYPE4)(PAR4); \
	FN r7 = FNCD; \
	Asm ( \
		"mov r7, %1\n\t" \
		"svc %2\n\t" \
		:"=r"(r0) \
		:"r"(r7),"I"(SERVICE_CALL_NUM),"0"(r0),"r"(r1),"r"(r2),"r"(r3)\
		:"memory","r7","lr" \
	); \
	return((TYPE)r0);

#define CAL_SVC_5M(TYPE, FNCD, TYPE1, PAR1, TYPE2, PAR2, \
						TYPE3, PAR3, TYPE4, PAR4, TYPE5, PAR5) \
	register TYPE1 r0 asm("r0") = (TYPE1)(PAR1); \
	register TYPE2 r1 asm("r1") = (TYPE2)(PAR2); \
	register TYPE3 r2 asm("r2") = (TYPE3)(PAR3); \
	register TYPE4 r3 asm("r3") = (TYPE4)(PAR4); \
	register TYPE5 r4 asm("r4") = (TYPE5)(PAR5); \
	FN r7 = FNCD; \
	Asm ( \
		"mov r7, %1\n\t" \
		"svc %2\n\t" \
		:"=r"(r0) \
		:"r"(r7),"I"(SERVICE_CALL_NUM),"0"(r0),"r"(r1),"r"(r2),"r"(r3),"r"(r4)\
		:"memory","r7","lr" \
	); \
	return((TYPE)r0);

#endif /* TOPPERS_CORE_SVC_H */
