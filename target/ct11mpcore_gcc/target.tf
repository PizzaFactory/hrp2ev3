$ 
$     パス2のターゲット依存テンプレート（CT11MPCore用）
$ 

$ 
$  スタートアップモジュールの定義
$ 
$START_OBJS = "start.o"$

$ 
$  有効な割込み番号，割込みハンドラ番号，CPU例外ハンドラ番号
$ 
$INTNO_VALID = { 0,1,...,45 }$
$INHNO_VALID = INTNO_VALID$

$ 
$  チップ依存テンプレートのインクルード
$ 
$INCLUDE "arch/arm_gcc/mpcore/arm11.tf"$
$INCLUDE "arch/gcc/ldscript.tf"$
