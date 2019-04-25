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
 *  $Id: core_config.h 876 2013-02-18 14:24:58Z ertl-hiro $
 */


/*
 *      ������¸�⥸�塼���ARM�ѡ�
 *
 *  ���Υ��󥯥롼�ɥե�����ϡ�target_config.h�ʤޤ��ϡ��������饤��
 *  �롼�ɤ����ե�����ˤΤߤ��饤�󥯥롼�ɤ���롥¾�Υե����뤫��
 *  ľ�ܥ��󥯥롼�ɤ��ƤϤʤ�ʤ���
 */

#ifndef TOPPERS_CORE_CONFIG_H
#define TOPPERS_CORE_CONFIG_H

/*
 *  ARM��¸�����
 */
#include "arm.h"

/*
 *  �������åȰ�¸�Υ��ꥪ�֥�������°���ʥ��顼�����å��ѡ�
 */
#define TARGET_MEMATR		(TA_SORDER|TA_WTHROUGH|TA_NONSHARED)

/*
 *  ���顼�����å���ˡ�λ���
 */
#define CHECK_STKSZ_ALIGN	8		/* �����å��������Υ��饤��ñ�� */
#define CHECK_USTKSZ_ALIGN	4096	/* �桼�������å��������Υ��饤��ñ�� */
#ifndef  __thumb__
#define CHECK_FUNC_ALIGN	4		/* �ؿ��Υ��饤��ñ�� */
#endif /* __thumb__ */
#define CHECK_FUNC_NONNULL			/* �ؿ�����NULL�����å� */
#define CHECK_STACK_ALIGN	4		/* �����å��ΰ�Υ��饤��ñ�� */
#define CHECK_STACK_NONNULL			/* �����å��ΰ����NULL�����å� */
#define CHECK_USTACK_ALIGN	4096	/* �桼�������å��ΰ�Υ��饤��ñ�� */
#define CHECK_USTACK_NONNULL		/* �桼�������å��ΰ����NULL�����å� */
#define CHECK_MPF_ALIGN		4		/* ����Ĺ����ס����ΰ�Υ��饤��ñ�� */
#define CHECK_MPF_NONNULL			/* ����Ĺ����ס����ΰ����NULL�����å� */
#define CHECK_MB_ALIGN		4		/* �����ΰ�Υ��饤��ñ�� */

/*
 *  �����å��ΰ�Υ���������Ƭ���Ϥ�����Ū����ˡ�ǻ���
 */
/* #define USE_TSKINICTXB */

/*
 *  ��åɥ�������������Ѥ���
 */
#define USE_REDZONE
#define TARGET_DUMMY_STKSZ		ARM_PAGE_SIZE

/*
 *  CPU��å��Ȥ���CPSR�Υѥ�����
 */
#ifndef CPSR_CPULOCK
#define CPSR_CPULOCK     (CPSR_IRQ_BIT)
#endif  /* CPSR_CPULOCK */

/*
 *  ����ߥ�å��Ȥ���CPSR�Υѥ�����
 */
#define CPSR_INTLOCK     (CPSR_FIQ_BIT|CPSR_IRQ_BIT)

/*
 *  CPSR�˾�˥��åȤ���ѥ�����
 */
#ifndef CPSR_ALWAYS_SET
#define CPSR_ALWAYS_SET  0x00
#endif  /* CPSR_ALWAYS_SET */

/*
 *  �㳰���ֹ�
 */
#define EXCH_NO_RESET     0
#define EXCH_NO_UNDEF     1
#define EXCH_NO_SVC       2
#define EXCH_NO_PABORT    3
#define EXCH_NO_DABORT    4
#define EXCH_NO_IRQ       5
#define EXCH_NO_FIQ       6
#define EXCH_NO_EMU_TEXRTN    7		/* �������㳰�¹Գ��ϻ������å����� */
#define EXCH_NO_EMU_RET_TEX   8		/* �������㳰�꥿����������å����� */

/*
 *  �㳰�θĿ�
 */  
#define TNUM_EXCH          9

#ifndef DEFAULT_SSTKSZ
#define DEFAULT_SSTKSZ		2048	/* �����ƥॹ���å��Υ����� */
#endif  /* DEFAULT_SSTKSZ */

/*
 *  SWI ������ֹ�����
 */
#define	SERVICE_CALL_NUM	1
#define	RET_TEX_NUM			2

/*
 *  �������㳰�ǻ��Ѥ���桼�������å�������
 */
#define TEX_USP_USE_BYTE	36

#ifndef TOPPERS_MACRO_ONLY

/* 
 *  �ݸ�ɥᥤ����������ƥ����ȥ֥�å���Ȥ�
 */
#define USE_DOMINICTXB

/*
 *  �ݸ�ɥᥤ����������ƥ����ȥ֥�å������
 */
typedef	struct domain_initialization_context_block {
	ID				domid;				/* �ɥᥤ��ID��ASID�Ȥ��ƻ��ѡ�*/
	const uint32_t	*p_section_table;	/* ���������ơ��֥�Υ��ɥ쥹 */
} DOMINICTXB;

/*
 *  �󥿥�������ƥ������ѤΥ����å������
 */
#define TOPPERS_ISTKPT(istk, istksz) ((STK_T *)((char *)(istk) + (istksz)))

/*
 *  �ץ��å����ü�̿��Υ���饤��ؿ����
 *  ARMCC�ζ�ͭ�Τ��ᡤ����ѥ���ΰ����˻��ꤵ�줿��Ǹ�������褦�ˤ��롥
 */
#include <core_insn.h>

/*
 *  ����������ƥ����ȥ֥�å������
 */
typedef struct task_context_block {
	void	*sp;	/* �����å��ݥ��� */
	FP		pc;		/* �ץ���५���� */
	bool_t	priv;	/* ���������ø��⡼�ɤ�¹Ԥ��Ƥ������true */
} TSKCTXB;

/*
 *  ���ɥ쥹�Ѵ��ơ��֥���ΰ�����
 */
extern const uint32_t section_table[][ARM_SECTION_TABLE_ENTRY];
extern const uint32_t page_table[][ARM_PAGE_TABLE_ENTRY];

/*
 *  TOPPERSɸ�����߽�����ǥ�μ¸�
 *
 *  IRQ�򥫡��ͥ�����⡤FIQ�򥫡��ͥ�������γ���ߤȤ��ư�����
 * 
 *  ARM��¸���Ǥϡ�TOPPERSɸ�����߽�����ǥ�Τ�����CPU��å�����
 *  �Τߤ��갷���������ͥ���٥ޥ�����������׵�ػߥե饰�˴ؤ���
 *  �ϡ��ƥ������åȰ�¸���Ǽ�갷��
 */

/*
 *  ����ƥ����Ȼ��ȤΤ�����ѿ�
 */
extern uint32_t excpt_nest_count; /* �㳰�ʳ����/CPU�㳰�ˤΥͥ��Ȳ���Υ������ */

/*
 *  ����ƥ����Ȥλ���
 *
 *  ARM�Ǥϡ�����������ƥ����Ȥ��󥿥�������ƥ����Ȥ�ξ���򥹡��ѡ�
 *  �Х������⡼�ɤ�ư����롥���Τ��ᡤCPSR�����ƤǤ�Ƚ�̤Ǥ��ʤ���
 *  ���Τ��ᡤ�㳰�ʳ����/CPU�㳰�ˤΥͥ��Ȳ���򥫥���Ȥ����ѿ�
 *  ��excpt_nest_count�ˤ��Ѱդ����㳰��������ǥ��󥯥���Ȥ��뤳��
 *  �ǡ�����ƥ����Ȥ�Ƚ�ꤹ�롥
 */
Inline bool_t
sense_context(void)
{
	return(excpt_nest_count > 0U);
}

/*
 *  CPU��å����֤ؤΰܹ�
 */
Inline void
x_lock_cpu(void)
{
	set_sr(current_sr() | CPSR_CPULOCK | CPSR_ALWAYS_SET);
	/* ����ƥ����륻������������ǥ��꤬�񤭴�����ǽ�������� */
	ARM_MEMORY_CHANGED;
}

#define t_lock_cpu()   x_lock_cpu()
#define i_lock_cpu()   x_lock_cpu()

/*
 *  CPU��å����֤β��
 */
Inline void
x_unlock_cpu(void)
{
	/* ����ƥ����륻������������ǥ��꤬�񤭴�����ǽ�������� */
	ARM_MEMORY_CHANGED;
	set_sr((current_sr() & (~CPSR_CPULOCK)) | CPSR_ALWAYS_SET);
}

#define t_unlock_cpu() x_unlock_cpu()
#define i_unlock_cpu() x_unlock_cpu()

/*
 *  CPU��å����֤λ���
 */
Inline bool_t
x_sense_lock(void)
{
	return((current_sr() & CPSR_CPULOCK) == CPSR_CPULOCK);
}

#define t_sense_lock() x_sense_lock()
#define i_sense_lock() x_sense_lock()

/*
 *  �ø��⡼�ɤ�¹Ԥ��Ƥ��뤫�ݤ���Ƚ��
 *
 *  �����δؿ��򡤥���饤��ؿ��ǤϤʤ��ޥ�������Ȥ��Ƥ���Τϡ���
 *  �λ����Ǥ�TCB���������Ƥ��ʤ�����Ǥ��롥
 */
#define t_sense_priv(p_tcb)    ((p_tcb)->tskctxb.priv)
#define i_sense_priv(p_tcb)    ((p_tcb)->tskctxb.priv)
#define i_sense_priv_runtsk()  (p_runtsk->tskctxb.priv)

/*
 *  �桼�������å��λĤꥵ�����Υ����å�
 *  ��p_runtsk�򻲾Ȥ��Ƥ��뤿��ޥ���ˤ���ɬ�פ����롣
 */
#define i_check_tex_runtsk()   \
		((current_usp()-(uint32_t)p_runtsk->p_tinib->ustk) < TEX_USP_USE_BYTE \
		? true : false)

/*
 *  �������ǥ����ѥå���
 */

/*
 *  �ǹ�ͥ���̥������ؤΥǥ����ѥå���core_support.S��
 *
 *  dispatch�ϡ�����������ƥ����Ȥ���ƤӽФ��줿�����ӥ������������
 *  ��ƤӽФ��٤���Τǡ�����������ƥ����ȡ�CPU��å����֡��ǥ����ѥ�
 *  �����ľ��֡��ʥ�ǥ��Ρ˳����ͥ���٥ޥ�����������֤ǸƤӽФ���
 *  ����Фʤ�ʤ���
 */
extern void dispatch(void);

/*
 *  �ǥ����ѥå����ư��ϡ�core_support.S��
 *
 *  start_dispatch�ϡ������ͥ뵯ư���˸ƤӽФ��٤���Τǡ����٤Ƥγ��
 *  �ߤ�ػߤ������֡ʳ���ߥ�å����֤�Ʊ���ξ��֡ˤǸƤӽФ��ʤ����
 *  �ʤ�ʤ���
 */
extern void start_dispatch(void) NoReturn;

/*
 *  ���ߤΥ���ƥ����Ȥ�ΤƤƥǥ����ѥå���core_support.S��
 *
 *  exit_and_dispatch�ϡ�ext_tsk����ƤӽФ��٤���Τǡ�����������ƥ�
 *  ���ȡ�CPU��å����֡��ǥ����ѥå����ľ��֡��ʥ�ǥ��Ρ˳����ͥ��
 *  �٥ޥ�����������֤ǸƤӽФ��ʤ���Фʤ�ʤ���
 */
extern void exit_and_dispatch(void) NoReturn;

/*
 *  �����ͥ�ν�λ�����θƽФ���core_support.S��
 *
 *  call_exit_kernel�ϡ������ͥ�ν�λ���˸ƤӽФ��٤���Τǡ��󥿥���
 *  ����ƥ����Ȥ��ڤ괹���ơ������ͥ�ν�λ������exit_kernel�ˤ�Ƥӽ�
 *  ����
 */
extern void call_exit_kernel(void) NoReturn;

/*
 *  �������ν�λ
 *
 *  �桼�����������ᥤ��롼���󤫤�꥿���󤷤����˸ƤФ��롼����
 *  ��ͭ�ΰ�����֤��롥 
 */
extern void call_ext_tsk(void) NoReturn  __attribute__((section(".text_shared")));

/*
 *  ����������ƥ����Ȥν����
 *
 *  ���������ٻ߾��֤���¹ԤǤ�����֤˰ܹԤ�����˸ƤФ�롥���λ���
 *  �ǥ����å��ΰ��ȤäƤϤʤ�ʤ���
 *
 *  activate_context�򡤥���饤��ؿ��ǤϤʤ��ޥ�������Ȥ��Ƥ���Τϡ�
 *  ���λ����Ǥ�TCB���������Ƥ��ʤ�����Ǥ��롥
 */
extern void start_stask_r(void);
extern void start_utask_r(void);

#define activate_context(p_tcb) \
{																		\
	(p_tcb)->tskctxb.sp = (void *)((char *)((p_tcb)->p_tinib->sstk)		\
										+ (p_tcb)->p_tinib->sstksz);	\
	if ((p_tcb)->p_tinib->p_dominib->domptn == TACP_KERNEL) {			\
		(p_tcb)->tskctxb.pc = (FP) start_stask_r;						\
	    (p_tcb)->tskctxb.priv = true;									\
	}																	\
	else {																\
		(p_tcb)->tskctxb.pc = (FP) start_utask_r;						\
	    (p_tcb)->tskctxb.priv = false;									\
	}																	\
}

/*
 *  calltex�ϻ��Ѥ��ʤ�
 */
#define OMIT_CALLTEX

/*
 * �������å����¸���˴ޤޤ��ɸ����㳰������ǽ�ν�����������Ѥ��ʤ�
 */
#define OMIT_INITIALIZE_EXCEPTION

/*
 * CPU�㳰�ϥ�ɥ�ν����
 */
Inline void
initialize_exception(void)
{
}

/*
 *  CPU�㳰��ȯ���������Υ����ƥ���֤λ���
 */

/*
 *  CPU�㳰��ȯ���������Υ���ƥ����Ȥλ���
 *
 *  CPU�㳰��ȯ���������Υ���ƥ����Ȥ�������������ƥ����Ȥλ���false��
 *  �����Ǥʤ�����true���֤���
 */
Inline bool_t
exc_sense_context(void *p_excinf)
{
	return(((exc_frame_t *)(p_excinf))->nest_count != 0U);
}

/*
 *  CPU�㳰��ȯ����������(��ǥ���)�����ͥ���٥ޥ����λ���
 */
Inline PRI
exc_get_ipm(void *p_excinf)
{
	return((PRI)(((exc_frame_t *)(p_excinf))->ipm));
}

/*
 *  CPU�㳰��ȯ����������CPSR�μ���
 */
Inline uint32_t
exc_get_sr(void *p_excinf){
	return(((exc_frame_t *)(p_excinf))->cpsr);
}

/*
 * CPU��å����֤�
 */
Inline bool_t
exc_sense_lock(void *p_excinf){
	return((exc_get_sr(p_excinf) & CPSR_CPULOCK) == CPSR_CPULOCK);
}

/*
 * ����ߥ�å����֤�
 */
Inline bool_t
exc_sense_int_lock(void *p_excinf){
	return((exc_get_sr(p_excinf) & CPSR_INTLOCK) == CPSR_INTLOCK);
}

/*
 *  CPU�㳰��ȯ���������Υ���ƥ����Ȥȳ���ߤΥޥ������֤λ���
 *
 *  CPU�㳰��ȯ���������Υ����ƥ���֤��������ͥ�¹���Ǥʤ�����������
 *  ��ƥ����ȤǤ��ꡤ������ߥ�å����֤Ǥʤ���CPU��å����֤Ǥʤ�����
 *  ����ͥ���٥ޥ�����������֤Ǥ������true�������Ǥʤ�����false���֤�
 *  ��CPU�㳰�������ͥ�������γ���߽������ȯ���������ˤ�false����
 *  ���ˡ�
 *
 */
Inline bool_t
exc_sense_intmask(void *p_excinf)
{
	return(!exc_sense_context(p_excinf)
		   && (exc_get_ipm(p_excinf) == 0U)
		   && !exc_sense_lock(p_excinf)
		   && !exc_sense_int_lock(p_excinf));
}

/*
 *  �ץ��å���¸�ν����
 */
extern void core_initialize(void);

/*
 *  �ץ��å���¸�ν�λ������
 */
extern void core_terminate(void);

/*
 *  �٥������ơ��֥��̿�ᤫ�黲�Ȥ���른�����襢�ɥ쥹
 * ��start.S��
 */
extern void *vector_ref_tbl;

/*
 *  CPU�㳰��ȯ�������Υ�����
 */
#ifdef SUPPORT_XLOG_SYS
extern void xlog_sys(void *p_excinf);
#endif /* SUPPORT_XLOG_SYS */

/*
 *  �㳰�٥�������ľ�ܼ¹Ԥ���ϥ�ɥ����Ͽ
 */ 
extern void x_install_exc(EXCNO excno, FP exchdr);

/*
 *  �㳰�ϥ�ɥ�
 */

/*
 *  ̤���̿�� �㳰�ϥ�ɥ��core_support.S��
 */
extern void undef_handler(void);

/*
 *  SVC �㳰�ϥ�ɥ��core_support.S��
 */
extern void svc_handler(void);

/*
 *  �ץ�ե��å����ܡ��� �㳰�ϥ�ɥ��core_support.S��
 */
extern void prefetch_handler(void);

/*
 *  �ǡ������ܡ��� �㳰�ϥ�ɥ��core_support.S��
 */
extern void data_abort_handler(void);

/*
 *  FIQ �㳰�ϥ�ɥ��core_support.S��
 */
extern void fiq_handler(void);

/*
 *  ̤������㳰�����ä����ν���
 */
extern void default_exc_handler(void *p_excinf);

/*
 *  �ץ�ե��å����ܡ����㳰�ϥ�ɥ�����
 */
extern void prefetch_handler_body(void);

/*
 *  �ǡ������ܡ����㳰�ϥ�ɥ�����
 */
extern void data_abort_handler_body(void *p_excinf);

/*
 *  �������㳰�¹Գ��ϻ������å������ϥ�ɥ�
 */
extern void emulate_texrtn_handler(void *p_excinf);

/*
 *  �������㳰�꥿����������å������ϥ�ɥ�
 */
extern void emulate_ret_tex_handler(void *p_excinf);

#if 0
/*
 *  ����񤭹��ߥ�����������Ƚ��
 */
extern bool_t probe_mem_write(void *base, SIZE size);

/*
 *  �����ɤ߹��ߥ�����������Ƚ��
 */
extern bool_t probe_mem_read(void *base, SIZE size);
#endif

/*
 *  �桼�������å��Υ����å�
 */
extern bool_t check_stack(void *base, SIZE size);

#endif /* TOPPERS_MACRO_ONLY */
#endif /* TOPPERS_CORE_CONFIG_H */
