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
 *  @(#) $Id: core_config.c 779 2012-09-08 04:58:07Z ertl-hiro $
 */

/*
 *        ������¸�⥸�塼���ARM�ѡ�
 */
#include "kernel_impl.h"
#include "task.h"
#include "memory.h"
#include "svc_manage.h"
#include "core_svc.h"
#include "kernel_cfg.h"

/*
 *  �ȥ졼�����ޥ���Υǥե�������
 */
#ifndef LOG_DSP_ENTER
#define LOG_DSP_ENTER(p_tcb)
#endif /* LOG_DSP_ENTER */

#ifndef LOG_DSP_LEAVE
#define LOG_DSP_LEAVE(p_tcb)
#endif /* LOG_DSP_LEAVE */

#ifndef LOG_EXTSVC_ENTER
#define LOG_EXTSVC_ENTER(fncd, par1, par2, par3, par4, par5)
#endif /* LOG_EXTSVC_ENTER */

#ifndef LOG_EXTSVC_LEAVE
#define LOG_EXTSVC_LEAVE(fncd, ercd)
#endif /* LOG_EXTSVC_LEAVE */

/*
 *  ����ƥ����Ȼ��ȤΤ�����ѿ�
 */
uint32_t excpt_nest_count;

/*
 *  �ץ��å���¸�ν����
 */
void
core_initialize()
{
	/*
	 *  �����ͥ뵯ư�����󥿥�������ƥ����ȤȤ���ư����뤿��1��
	 */ 
	excpt_nest_count = 1;
}

/*
 *  �ץ��å���¸�ν�λ����
 */
void
core_terminate(void)
{

}

/*
 *  �������ν�λ
 *
 *  �桼�����������ᥤ��롼���󤫤�꥿���󤷤����˸ƤФ��롼����
 */

/* ��ͭ�ΰ������ */
void call_ext_tsk(void) __attribute__((section(".text_shared")));

void
call_ext_tsk(void)
{
	register ER r0 asm("r0");
	register FN r7 asm("r7");

	/* ext_tsk�θƤӽФ� */
	r7 = (FN)(TFN_EXT_TSK);
	Asm (
		"svc %1\n\t"
		:"=r"(r0)
		:"I"(SERVICE_CALL_NUM),"r"(r7)
		:"lr"
	);

	/* ext_ker�θƤӽФ� */
	r7 = (FN)(TFN_EXT_KER);
	Asm (
		"svc %1\n\t"
		:"=r"(r0)
		:"I"(SERVICE_CALL_NUM),"r"(r7)
		:"lr"
	);

	assert(0);
	while (true);		/* ����ѥ���ηٹ��к� */
}

/*
 *	�������㳰�����롼���󤫤�Υ꥿����
 */
/* ��ͭ�ΰ������ */
void call_assert_ret_tex(void) __attribute__((section(".text_shared")));

void
call_assert_ret_tex(void)
{
	register ER r0 asm("r0");
	register FN r7 asm("r7");

	/* ext_tsk�θƤӽФ� */
	r7 = (FN)(TFN_EXT_TSK);
	Asm (
		"svc %1\n\t"
		:"=r"(r0)
		:"I"(SERVICE_CALL_NUM),"r"(r7)
		:"lr"
	);

	/* ext_ker�θƤӽФ� */
	r7 = (FN)(TFN_EXT_KER);
	Asm (
		"svc %1\n\t"
		:"=r"(r0)
		:"I"(SERVICE_CALL_NUM),"r"(r7)
		:"lr"
	);

	assert(0);
}

/*
 *  CPU�㳰��ȯ�������Υ�����
 *
 *  CPU�㳰�ϥ�ɥ���椫�顤CPU�㳰����ݥ��󥿡�p_excinf�ˤ�����Ȥ�
 *  �ƸƤӽФ����Ȥǡ�CPU�㳰��ȯ�������򥷥��ƥ���˽��Ϥ��롥
 */
#ifdef SUPPORT_XLOG_SYS

void
xlog_sys(void *p_excinf)
{
	uint32_t excno      = ((exc_frame_t *)(p_excinf))->excno;
	uint32_t nest_count = ((exc_frame_t *)(p_excinf))->nest_count;
	uint32_t ipm        = ((exc_frame_t *)(p_excinf))->ipm;
	uint32_t r0         = ((exc_frame_t *)(p_excinf))->r0;
	uint32_t r1         = ((exc_frame_t *)(p_excinf))->r1;
	uint32_t r2         = ((exc_frame_t *)(p_excinf))->r2;
	uint32_t r3         = ((exc_frame_t *)(p_excinf))->r3;
	uint32_t r12        = ((exc_frame_t *)(p_excinf))->r12;
	uint32_t lr         = ((exc_frame_t *)(p_excinf))->lr;
    uint32_t pc         = ((exc_frame_t *)(p_excinf))->pc;
	uint32_t cpsr       = ((exc_frame_t *)(p_excinf))->cpsr;

	syslog(LOG_EMERG, " excno = %d, excpt_nest_count = %d, ipm = 0x%08x ",
		   excno, nest_count, ipm);
	syslog(LOG_EMERG, " r0 = 0x%08x,  r1 = 0x%08x, r2 = 0x%08x, r3 = 0x%08x ",
		   r0, r1, r2, r3);
	syslog(LOG_EMERG, " r12 = 0x%08x, lr = 0x%08x, pc = 0x%08x, cpsr = 0x%08x ",
		   r12, lr, pc, cpsr);
}

#endif /* SUPPORT_XLOG_SYS */

/*
 *  �㳰�٥�������ľ�ܼ¹Ԥ���ϥ�ɥ����Ͽ
 */ 
void
x_install_exc(EXCNO excno, FP exchdr)
{
	*(((FP*)vector_ref_tbl) + excno) = exchdr;
}

#ifndef OMIT_DEFAULT_EXC_HANDLER
/*
 * ̤������㳰�����ä����ν���
 */
void
default_exc_handler(void *p_excinf)
{
	syslog(LOG_EMERG, "Unregistered Exception occurs.");
#ifdef SUPPORT_XLOG_SYS
	xlog_sys(p_excinf);
#endif /* SUPPORT_XLOG_SYS */
	ext_ker();
}
#endif /* OMIT_DEFAULT_EXC_HANDLER */

/*
 * �ץ�ե��å����ܡ����㳰�ϥ�ɥ�����
 *
 *  ����������ȿ�㳰������¾�Υץ�ե��å��㳰�ǽ�����ʬ����
 */
void
prefetch_handler_body(void)
{
	uint32_t ifsr;

	/* �ե�����ȥ��ơ������쥸�������ͤ���� */
	Asm("mrc p15, 0, %0, c5, c0, 1":"=r"(ifsr));
	/* �����������İ�ȿ����Ƚ�� */
	if(((ifsr & (0x40F)) == (0xD)) || ((ifsr & (0x40F)) == (0xF))) {
		syslog(LOG_EMERG, "Memory access violation (rx) occurs.");
		while(1);
	}

	syslog(LOG_EMERG, "Prefetch exception occurs.");
	while(1);
}

/*
 *  �ǡ������ܡ����㳰�ϥ�ɥ�����
 *
 *  ����������ȿ�㳰������¾�Υǡ������ܡ����㳰�ǽ�����ʬ����
 */
void
data_abort_handler_body(void *p_excinf)
{
	uint32_t dfsr;

	/* �ե�����ȥ��ơ������쥸�������ͤ���� */
	Asm("mrc p15, 0, %0, c5, c0, 0":"=r"(dfsr));
	/* �����������İ�ȿ����Ƚ�� */
	if(((dfsr & (0x40F)) == (0xD)) || ((dfsr & (0x40F)) == (0xF))) {
		/* �ɤ߽Ф������������񤭹��ߥ�����������Ƚ�� */
		if((dfsr & (0x800)) == (0x800)) {
			syslog(LOG_EMERG, "Memory access violation (w) occurs.");
			while(1);
		} else {
			syslog(LOG_EMERG, "Memory access violation (rx) occurs.");
			while(1);
		}
	}

    uint32_t far;
	asm("mrc p15, 0, %0, c6, c0, 0":"=r"(far));

    switch(dfsr & 0xF) {
    case 0x5:
	    syslog(LOG_EMERG, "Section translation fault occurs @ 0x%08x.", far);
        break;

    case 0x7:
	    syslog(LOG_EMERG, "Page translation fault occurs @ 0x%08x.", far);
        break;
    }

	syslog(LOG_EMERG, "Data abort exception occurs. DFSR: 0x%x", dfsr);
	//syslog(LOG_EMERG, "Data abort exception occurs.");
	xlog_sys(p_excinf);
	while(1);
}

/*
 *  �������㳰�¹Գ��ϻ������å������ϥ�ɥ�
 */
void
emulate_texrtn_handler(void *p_excinf)
{
    syslog(LOG_EMERG, "User stack is no space at prepare texrtn.");
    while(1);
}

/*
 *  �������㳰�꥿����������å������ϥ�ɥ�
 */
void
emulate_ret_tex_handler(void *p_excinf)
{
    syslog(LOG_EMERG, "User stack is no space at return texrtn.");
    while(1);
}

/*
 * check_stack : �桼�������å��Υ����å�
 *   �������ơ�
 *     �����Υݥ��󥿤Υ��饤���ȤΥ����å��ȡ�
 *     �����Υݥ��󥿤�������Υ�����ʬ���ΰ褬�桼�������å��ΰ��⤫��
 *     �����å�
 *   ������
 *     �����å���Ԥ��ݥ��󥿡������å���Ԥ�������
 *   ����͡�
 *     true  �桼�������å��ΰ���
 *     false �桼�������å��ΰ賰�ʥ��顼��
 *
 *   ��PROBE_STACK ��Ʊ���ν�����������֥餫��ƤӽФ�ɬ�פ����뤿���
 *     ���Ȥ��롣
 */
bool_t
check_stack(void *base, SIZE size)
{
	return( ((((SIZE)(base)) & (sizeof(uintptr_t) - 1)) == 0U
			 && probe_stack(base, size)) );
}
