$FILE "module_cfg.c"$
#include <kernel.h>$NL$
#include "common/module_common.h"$NL$
#include "target_config.h"$NL$
$INCLUDES$

$tnum_mod_cfg_entries = 0$

$ ==================== 
$ Handle CRE_TSK
$ ====================

$
$ Generate IDs and user stacks
$ 
$FOREACH id TSK.ID_LIST$
$FILE "module_cfg.h"$
    extern ID _module_id_$id$;$NL$
    #define $id$ ((const ID)(_module_id_$id$))$NL$
$FILE "module_cfg.c"$
    ID _module_id_$id$ __attribute__((section (".module.text")));$NL$
    static STK_T _module_ustack_$id$[COUNT_STK_T($TSK.STKSZ[id]$)];$NL$
    $IF EQ(TSK.STK[id], "NULL")$
        $TSK.STK[id] = FORMAT("_module_ustack_%1%", id)$
        $TSK.STKSZ[id] = FORMAT("ROUND_STK_T(%1%)", TSK.STKSZ[id])$
    $ELSE$
	    $ERROR TSK.TEXT_LINE[i]$E_RSATR: $FORMAT(_("stk in CRE_TSK must be NULL"))$$END$
    $END$
$END$

$
$ Generate '_module_ctsk_tab' and update '_module_cfg_tab'
$
$FILE "module_cfg.c"$
$index = 0$
static const T_CTSK _module_ctsk_tab[$LENGTH(TSK.ID_LIST)$] = {$NL$
$FOREACH id TSK.ID_LIST$
    $IF !LENGTH(TSK.SSTKSZ[id])$
        $TSK.SSTKSZ[id] = "DEFAULT_SSTKSZ"$
    $END$
    $IF !LENGTH(TSK.SSTK[id])$
        $TSK.SSTK[id] = "NULL"$
    $END$
    $TAB${ $TSK.TSKATR[id]$, $TSK.EXINF[id]$, $TSK.TASK[id]$, $TSK.ITSKPRI[id]$, $TSK.STKSZ[id]$, $TSK.STK[id]$, $TSK.SSTKSZ[id]$, $TSK.SSTK[id]$ },$NL$
    $MODCFGTAB.SFNCD[tnum_mod_cfg_entries] = "TSFN_CRE_TSK"$
    $MODCFGTAB.ARGUMENT[tnum_mod_cfg_entries] = FORMAT("&_module_ctsk_tab[%1%]",index)$
    $MODCFGTAB.RETVALPTR[tnum_mod_cfg_entries] = FORMAT("&_module_id_%1%",id)$
    $tnum_mod_cfg_entries = tnum_mod_cfg_entries + 1$
    $index = index + 1$
$END$
};$NL$

$MODCFGTAB_SFNCD[1] = VALUE("x", 0)$
$DEFAULT.ACPTN[1] = VALUE("TACP_KERNEL", 1)$
$ $TRACE(MODCFGTAB_SFNCD)$
$ $TRACE(DEFAULT.ACPTN[1])$

$ ===========================
$ Generate '_module_cfg_tab'
$ ===========================
$NL$
const SIZE _module_cfg_entry_num = $tnum_mod_cfg_entries$;$NL$
$NL$
const MOD_CFG_ENTRY _module_cfg_tab[$tnum_mod_cfg_entries$] = {$NL$
$FOREACH idx RANGE(0,tnum_mod_cfg_entries - 1)$
    $TAB${ $MODCFGTAB.SFNCD[idx]$, $MODCFGTAB.ARGUMENT[idx]$, $MODCFGTAB.RETVALPTR[idx]$ },$NL$
$END$
};

$ $TRACE(TSK.ID_LIST)$
