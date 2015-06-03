$ 
$     パス2のチップ依存テンプレート（MPCore用）
$ 

$ 
$  ATT_ISRで使用できる割込み番号とそれに対応する割込みハンドラ番号
$ 
$INTNO_ATTISR_VALID = INTNO_VALID$
$INHNO_ATTISR_VALID = INHNO_VALID$

$ 
$  DEF_INTで使用できる割込みハンドラ番号
$ 
$INHNO_DEFINH_VALID = INHNO_VALID$

$ 
$  CFG_INTで使用できる割込み番号と割込み優先度
$ 
$INTNO_CFGINT_VALID  = INTNO_VALID$

$ 
$  セクションのアライン単位
$ 
$TARGET_SEC_ALIGN_STR = "4"$

$ 
$  ページサイズ
$ 
$TARGET_PAGE_SIZE_STR = "4K"$

$ 
$  標準のセクションのメモリオブジェクト属性の定義
$ 
$MEMATR_TEXT = TA_NOWRITE|TA_EXEC$
$MEMATR_RODATA = TA_NOWRITE$
$MEMATR_DATA = TA_MEMINI$
$MEMATR_BSS = TA_NULL$
$MEMATR_PRSV = TA_MEMPRSV$

$ 
$  ユーザスタック領域のメモリオブジェクト属性
$ 
$TARGET_MEMATR_USTACK = TA_MEMPRSV$

$ 
$  固定長メモリプール領域のメモリオブジェクト属性
$ 
$TARGET_MEMATR_MPFAREA = TA_MEMPRSV$

$ 
$  ARM依存テンプレートのインクルード
$ 
$INCLUDE"arm_gcc/common/core.tf"$

$ 
$  割込みハンドラテーブル
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
$  割込み属性テーブル
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
