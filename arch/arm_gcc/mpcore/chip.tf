$ 
$     �ѥ�2�Υ��åװ�¸�ƥ�ץ졼�ȡ�MPCore�ѡ�
$ 

$ 
$  ATT_ISR�ǻ��ѤǤ��������ֹ�Ȥ�����б��������ߥϥ�ɥ��ֹ�
$ 
$INTNO_ATTISR_VALID = INTNO_VALID$
$INHNO_ATTISR_VALID = INHNO_VALID$

$ 
$  DEF_INT�ǻ��ѤǤ������ߥϥ�ɥ��ֹ�
$ 
$INHNO_DEFINH_VALID = INHNO_VALID$

$ 
$  CFG_INT�ǻ��ѤǤ��������ֹ�ȳ����ͥ����
$ 
$INTNO_CFGINT_VALID  = INTNO_VALID$

$ 
$  ���������Υ��饤��ñ��
$ 
$TARGET_SEC_ALIGN_STR = "4"$

$ 
$  �ڡ���������
$ 
$TARGET_PAGE_SIZE_STR = "4K"$

$ 
$  ɸ��Υ��������Υ��ꥪ�֥�������°�������
$ 
$MEMATR_TEXT = TA_NOWRITE|TA_EXEC$
$MEMATR_RODATA = TA_NOWRITE$
$MEMATR_DATA = TA_MEMINI$
$MEMATR_BSS = TA_NULL$
$MEMATR_PRSV = TA_MEMPRSV$

$ 
$  �桼�������å��ΰ�Υ��ꥪ�֥�������°��
$ 
$TARGET_MEMATR_USTACK = TA_MEMPRSV$

$ 
$  ����Ĺ����ס����ΰ�Υ��ꥪ�֥�������°��
$ 
$TARGET_MEMATR_MPFAREA = TA_MEMPRSV$

$ 
$  ARM��¸�ƥ�ץ졼�ȤΥ��󥯥롼��
$ 
$INCLUDE"arm_gcc/common/core.tf"$

$ 
$  ����ߥϥ�ɥ�ơ��֥�
$ 
$NL$
const FP _kernel_inh_tbl[TNUM_INH] = {$NL$
$FOREACH inhno INHNO_VALID$ 
	$IF LENGTH(INH.INHNO[inhno])$
		$TAB$(FP)($INH.INTHDR[inhno]$),
	$ELSE$
		$TAB$(FP)(_kernel_default_int_handler),
	$END$
	$SPC$$FORMAT("/* %d */", +inhno)$$NL$
$END$
$NL$};$NL$
$NL$

$ 
$  �����°���ơ��֥�
$ 
$NL$
const uint8_t _kernel_cfgint_tbl[TNUM_INH] = {$NL$
$FOREACH inhno INHNO_VALID$ 
	$IF LENGTH(INH.INHNO[inhno])$
		$TAB$1U,
	$ELSE$
		$TAB$0U,
	$END$
	$SPC$$FORMAT("/* 0x%03x */", inhno)$$NL$
$END$
$NL$};$NL$
$NL$
