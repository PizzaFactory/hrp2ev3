$ ======================================================================
$
$   TOPPERS/HRP Kernel
$       Toyohashi Open Platform for Embedded Real-Time Systems/
$       High Reliable system Profile Kernel
$
$   Copyright (C) 2011-2014 by Embedded and Real-Time Systems Laboratory
$               Graduate School of Information Science, Nagoya Univ., JAPAN
$  
$   �嵭����Ԥϡ��ʲ���(1)��(4)�ξ������������˸¤ꡤ�ܥ��եȥ���
$   �����ܥ��եȥ���������Ѥ�����Τ�ޤࡥ�ʲ�Ʊ���ˤ���ѡ�ʣ������
$   �ѡ������ۡʰʲ������ѤȸƤ֡ˤ��뤳�Ȥ�̵���ǵ������롥
$   (1) �ܥ��եȥ������򥽡��������ɤη������Ѥ�����ˤϡ��嵭������
$       ��ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ��꤬�����Τޤޤη��ǥ���
$       ����������˴ޤޤ�Ƥ��뤳�ȡ�
$   (2) �ܥ��եȥ������򡤥饤�֥������ʤɡ�¾�Υ��եȥ�������ȯ�˻�
$       �ѤǤ�����Ǻ����ۤ�����ˤϡ������ۤ�ȼ���ɥ�����ȡ�����
$       �ԥޥ˥奢��ʤɡˤˡ��嵭�����ɽ�����������Ѿ�浪��Ӳ���
$       ��̵�ݾڵ����Ǻܤ��뤳�ȡ�
$   (3) �ܥ��եȥ������򡤵�����Ȥ߹���ʤɡ�¾�Υ��եȥ�������ȯ�˻�
$       �ѤǤ��ʤ����Ǻ����ۤ�����ˤϡ����Τ����줫�ξ�����������
$       �ȡ�
$     (a) �����ۤ�ȼ���ɥ�����ȡ����Ѽԥޥ˥奢��ʤɡˤˡ��嵭����
$         �ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ����Ǻܤ��뤳�ȡ�
$     (b) �����ۤη��֤��̤�������ˡ�ˤ�äơ�TOPPERS�ץ������Ȥ�
$         ��𤹤뤳�ȡ�
$   (4) �ܥ��եȥ����������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������뤤���ʤ�»
$       ������⡤�嵭����Ԥ����TOPPERS�ץ������Ȥ����դ��뤳�ȡ�
$       �ޤ����ܥ��եȥ������Υ桼���ޤ��ϥ���ɥ桼������Τ����ʤ���
$       ͳ�˴�Ť����ᤫ��⡤�嵭����Ԥ����TOPPERS�ץ������Ȥ�
$       ���դ��뤳�ȡ�
$  
$   �ܥ��եȥ������ϡ�̵�ݾڤ��󶡤���Ƥ����ΤǤ��롥�嵭����Ԥ�
$   ���TOPPERS�ץ������Ȥϡ��ܥ��եȥ������˴ؤ��ơ�����λ�����Ū
$   ���Ф���Ŭ������ޤ�ơ������ʤ��ݾڤ�Ԥ�ʤ����ޤ����ܥ��եȥ���
$   �������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������������ʤ�»���˴ؤ��Ƥ⡤��
$   ����Ǥ�����ʤ���
$
$   $Id: kernel_lib.tf 1003 2014-04-23 11:58:48Z ertl-hiro $
$  
$ =====================================================================

$ =====================================================================
$  ���ꥪ�֥������Ȥ���Ƭ�������Υ��ɥ쥹�μ��Ф�
$ =====================================================================

$IF !ISFUNCTION("START_SYMBOL")$
$FUNCTION START_SYMBOL$
	$RESULT = SYMBOL(CONCAT("__start_", ARGV[1]))$
$END$
$END$

$IF !ISFUNCTION("LIMIT_SYMBOL")$
$FUNCTION LIMIT_SYMBOL$
	$RESULT = SYMBOL(CONCAT("__limit_", ARGV[1]))$
$END$
$END$

$ =====================================================================
$  �ǡ�������������LMA����VMA�ؤΥ��ԡ�
$ =====================================================================

$FUNCTION COPY_LMA$
	$FOREACH lma LMA.ORDER_LIST$
		$start_data = SYMBOL(LMA.START_DATA[lma])$
		$end_data = SYMBOL(LMA.END_DATA[lma])$
		$start_idata = SYMBOL(LMA.START_IDATA[lma])$
		$IF !LENGTH(start_data)$
			$ERROR$
				$FORMAT(_("symbol '%1%' not found"), LMA.START_DATA[lma])$
			$END$
		$ELIF !LENGTH(end_data)$
			$ERROR$
				$FORMAT(_("symbol '%1%' not found"), LMA.END_DATA[lma])$
			$END$
		$ELIF !LENGTH(start_idata)$
			$ERROR$
				$FORMAT(_("symbol '%1%' not found"), LMA.START_IDATA[lma])$
			$END$
		$ELSE$
			$BCOPY(start_idata, start_data, end_data - start_data)$
		$END$
	$END$
$END$

$ =====================================================================
$ kernel_cfg.c��kernel_mem.c�ζ�����ʬ������
$ =====================================================================

$FUNCTION GENERATE_CFILE_HEADER$
	#include "kernel/kernel_int.h"$NL$
	#include "kernel_cfg.h"$NL$
	$NL$
	#if TKERNEL_PRID != 0x06u$NL$
	#error "The kernel does not match this configuration file."$NL$
	#endif$NL$
	$NL$

	/*$NL$
	$SPC$*  Include Directives (#include)$NL$
	$SPC$*/$NL$
	$NL$
	$INCLUDES$
	$NL$
$END$

$ =====================================================================
$ �ݸ�ɥᥤ��������������
$ =====================================================================

$FUNCTION GENERATE_DOMINIB$
	/*$NL$
	$SPC$*  Protection Domain Management Functions$NL$
	$SPC$*/$NL$
	$NL$

$	// �ݸ�ɥᥤ��ID�ֹ�κ�����
	const ID _kernel_tmax_domid = (TMIN_DOMID + TNUM_DOMID - 1);$NL$
	$NL$

$	// �ݸ�ɥᥤ����������ƥ����ȥ֥�å��Τ�������
	$IF ISFUNCTION("PREPARE_DOMINICTXB")$
		$PREPARE_DOMINICTXB()$
	$END$

$	// �����ͥ�ɥᥤ����ݸ�ɥᥤ�������֥�å�������
	const DOMINIB _kernel_dominib_kernel = { TACP_KERNEL
	, INT_PRIORITY(TMIN_TPRI)
	$IF USE_DOMINICTXB$
		, $DOMINICTXB_KERNEL$
	$END$
	$SPC$};$NL$
	$NL$

$	// �ݸ�ɥᥤ�������֥�å�������
	$IF LENGTH(DOM.ID_LIST)$
		const DOMINIB _kernel_dominib_table[TNUM_DOMID] = {$NL$
		$JOINEACH domid DOM.ID_LIST ",\n"$
			$TAB${ TACP($domid$)
			, INT_PRIORITY($ALT(MINPRIORITY[domid], "TMIN_TPRI + 1")$)
			$IF USE_DOMINICTXB$
				, $GENERATE_DOMINICTXB(domid)$
			$END$
			$SPC$}
		$END$$NL$
		};$NL$
	$ELSE$
		TOPPERS_EMPTY_LABEL(const DOMINIB, _kernel_dominib_table);$NL$
	$END$$NL$
$END$
