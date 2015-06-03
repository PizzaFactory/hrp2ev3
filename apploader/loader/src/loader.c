/*
 * loader.c
 *
 *  Created on: Nov 8, 2013
 *      Author: liyixiao
 */

#include <kernel.h>
#include <t_syslog.h>
#include "kernel/kernel_impl.h"
#include <kernel_cfg.h>
#include <string.h>
//#include "kernel/check.h"
#include "dmloader.h"
#include "dmloader_impl.h"
#include "elf.h"

//#include "app.c"

static ER elf32_trans_prepare(elf32_load_trans *elt, const uint8_t *elf32_data);
static ER elf32_trans_load(elf32_load_trans *elt);

static QUEUE free_lkmcb;
//static ID dmloader_mtx;

#define DMLOADER_MTX DMLOADER_MTX

/**
 * Names:
 * 1) Dynamic Module Loader, dmloader
 * 2) Loadable Module, LDM
 * 3) Dynamic User Domain, dud
 *
 * Static API:
 * DEF_DUD
 *
 * Service call:
 * ERID dmloader_ins_ldm(const uint8_t *mod_data, ID domid);
 * 0) Mutex
 * 1) Allocate LDMCB
 * 2) Load ELF32 data => Perform ELF32 load transaction
 * 3) Modify LDMCB
 * 4) Run init
 *
 * ER dmloader_act_ldm(ID ldmid);
 *
 * ER dmloader_run_lkm(const uint8_t *mod_data, intptr_t exinf)
 *
 * Issue:
 * 1) module_initialize() runs in dmloader_act_ldm()'s caller's context which LACKS SECURITY.
 * 2) What if data segment doesn't exist
 */

/**
 * TODO: should handle by rename
 */
#define ldmcb_table _dmloader_ldmcb_table
#define lum_domid_table _dmloader_lum_domid_table

//void dmloader_initialize(intptr_t unused) {
//    for(size_t i = 0; i < TNUM_LDMID; ++i) {
//        ldmcb_table[i].status = LDMCB_FREE;
//    }
//
//    queue_initialize(&free_lkmcb);
//    for (size_t i = TMIN_LKMID; i < TMIN_LKMID + TNUM_LKMID; i++)
//        queue_insert_prev(&free_lkmcb, &(ldmcb_table[i].free_queue));
//}

/**
 * Check whether a memory area is in an LDM container.
 */
inline static bool_t
probe_ldm_memory(const void *base, SIZE size, T_LDM_CAN *ldm_can) {
	if(base >= ldm_can->text_mempool && base + size <= ldm_can->text_mempool + ldm_can->text_mempool_size)
		return true;
	if(base >= ldm_can->data_mempool && base + size <= ldm_can->data_mempool + ldm_can->data_mempool_size)
		return true;
	return false;
}

static ER
handle_module_cfg_tab(T_LDM_CAN *ldm_can) {
	// TODO: check cfg table memory
	assert(ldm_can->cfg_entry_num > 0 && ldm_can->cfg_table != NULL);
	assert(probe_ldm_memory(ldm_can->cfg_table, sizeof(MOD_CFG_ENTRY) * ldm_can->cfg_entry_num, ldm_can));

	ER_ID ercd = E_OK;

	// Creation stage
	for(SIZE i = 0; i < ldm_can->cfg_entry_num && ercd == E_OK; ++i) {
		MOD_CFG_ENTRY *ent = &ldm_can->cfg_table[i];
		switch(ent->sfncd) {
		case TSFN_CRE_TSK: {
            syslog(LOG_ERROR, "[dmloader] MOD_CFG_ENTRY TSFN_CRE_TSK");
			assert(probe_ldm_memory(ent->argument, sizeof(T_CTSK), ldm_can));
			assert(probe_ldm_memory(ent->retvalptr, sizeof(ID), ldm_can));
			T_CTSK pk_ctsk = *(T_CTSK*)ent->argument;
			assert(probe_ldm_memory(pk_ctsk.stk, pk_ctsk.stksz, ldm_can)); // Check user stack
			assert(pk_ctsk.sstk == NULL); // Check system stack
			pk_ctsk.tskatr &= ~TA_ACT; // Clear TA_ACT
			assert(get_atrdomid(pk_ctsk.tskatr) == TDOM_SELF); // Check original DOMID
			pk_ctsk.tskatr |= TA_DOM(ldm_can->domid); // Set new DOMID
			ercd = acre_tsk(&pk_ctsk);
			assert(ercd > 0);
			if(ercd > 0) {
			    *(ID*)ent->retvalptr = ercd;
			    ercd = E_OK;
			}
			break; }
		default:
		    syslog(LOG_ERROR, "[dmloader] Unsupported static function code %d.", ent->sfncd);
		    ercd = E_OBJ;
		}
	}

	// Rollback stage
	assert(ercd == E_OK);
	syslog(LOG_ERROR, "[dmloader] text paddr: 0x%x, data paddr: 0x%x", ldm_can->text_mempool, ldm_can->data_mempool);

	// Acting stage
    for(SIZE i = 0; i < ldm_can->cfg_entry_num; ++i) {
        MOD_CFG_ENTRY *ent = &ldm_can->cfg_table[i];
        switch(ent->sfncd) {
        case TSFN_CRE_TSK: {
        	T_CTSK pk_ctsk = *(T_CTSK*)ent->argument;
        	if(pk_ctsk.tskatr & TA_ACT) {
        		ercd = act_tsk(*(ID*)ent->retvalptr);
            	assert(ercd == E_OK);
        	}
            break; }
        default:
            syslog(LOG_ERROR, "[dmloader] Unsupported static function code %d.", ent->sfncd);
        }
    }

    return ercd;
}


/**
 * service call
 * task context
 *
 * ER:
 * E_ID: not a user domain
 * E_OBJ: Wrong ELF data
 */
ER_ID dmloader_ins_ldm(const uint8_t *mod_data, T_LDM_CAN *ldm_can) {
	//TODO: hard coding
//    mod_data = untitled;

    ER_ID ercd;
    LDMCB *p_ldmcb = NULL;
    elf32_load_trans elt;
    elt.text_mem = NULL;
    elt.data_mem = NULL;

    ercd = loc_mtx(DMLOADER_MTX);
    if(ercd != E_OK) {
        syslog(LOG_ERROR, "[dmloader] Acquire mutex failed.");
        goto error_exit;
    }

    /**
     * Find a free LDMCB
     * TODO:
     * 1) check context
     * 2) lock CPU to modify 'free_dmcb' ? => overhead too long for time tick ?
     * 3) check domid
     */
//    if(domid == TDOM_KERNEL) { // Loadable Kernel Module
//        if(!queue_empty(&free_lkmcb))
//            p_ldmcb = queue_delete_next(&free_lkmcb);
//    } else {                   // Loadable User Module
//        for(size_t i = 0; i < TNUM_LUMID; ++i)
//            if(lum_domid_table[i] == domid) {
//                p_ldmcb = &ldmcb_table[i];
//                if(p_ldmcb->status != LDMCB_FREE) {
//                    syslog(LOG_ERROR, "[dmloader] Dynamic user domain is already in use.");
//                    ercd = E_OBJ;
//                    goto error_exit;
//                }
//                break;
//            }
//    }
//    if(p_ldmcb == NULL) {
//        ercd = E_NOID;
//        goto error_exit;
//    }

    /**
     * ELF32 load transaction: Prepare step
     */
    ercd = elf32_trans_prepare(&elt, mod_data);
    if(ercd != E_OK)
        goto error_exit;

    /**
     * ELF32 load transaction: Allocate step
     */
    if(elt.text_segment->p_memsz > ldm_can->text_mempool_size) {
        syslog(LOG_ERROR, "[dmloader] Container's text memory pool is too small.");
        ercd = E_NOMEM;
        goto error_exit;
    }
    if(elt.data_segment->p_memsz > ldm_can->data_mempool_size) {
        syslog(LOG_ERROR, "[dmloader] Container's data memory pool is too small.");
        ercd = E_NOMEM;
        goto error_exit;
    }
    elt.text_mem = ldm_can->text_mempool;
    elt.data_mem = ldm_can->data_mempool;

    /**
     * ELF32 load transaction: Load step
     */
    ercd = elf32_trans_load(&elt);
    if(ercd != E_OK)
        goto error_exit;

	dmloader_instruction_memory_barrier();

    /**
     * Locate module configuration table
     */
    ldm_can->cfg_table = NULL;
    ldm_can->cfg_entry_num = 0;
    for(size_t i = 0; i < elt.symtab->sh_size / elt.symtab->sh_entsize; ++i) {
        const Elf32_Ehdr *ehdr = elt.elf_data;
        const Elf32_Sym* sym = ELF32_SYM(ehdr, elt.symtab, i);

        if(ELF32_ST_TYPE(sym->st_info) == STT_OBJECT) {
            if(strcmp(ELF32_STR_NAME(ehdr, elt.strtab, sym->st_name), "_module_cfg_tab") == 0)
                ldm_can->cfg_table = elf32_vaddr_to_paddr(&elt, sym->st_value);
            else if(strcmp(ELF32_STR_NAME(ehdr, elt.strtab, sym->st_name), "_module_cfg_entry_num") == 0)
                ldm_can->cfg_entry_num = *(SIZE*)elf32_vaddr_to_paddr(&elt, sym->st_value);
        }
    }
    if(ldm_can->cfg_table == NULL || ldm_can->cfg_entry_num == 0) {
        syslog(LOG_ERROR, "[dmloader] Module configuration table is corrupted.");
        ercd = E_OBJ;
        goto error_exit;
    }
    syslog(LOG_ERROR, "[dmloader] Module configuration table size %d.", ldm_can->cfg_entry_num);

    ercd = handle_module_cfg_tab(ldm_can);

    /**
     * Update LDMCB
     */
//    p_ldmcb->status = LDMCB_READY;
//    p_ldmcb->text_mem = elt.text_mem;
//    p_ldmcb->data_mem = elt.data_mem;
//    p_ldmcb->entry_point = elt.entry_point;
//    p_ldmcb->exit_point = elt.exit_point;
//
//    /**
//     * Call module_initialize()
//     * TODO: call context ?
//     */
//    p_ldmcb->status = LDMCB_RUNNING;
//    p_ldmcb->entry_point(exinf);

    ercd = unl_mtx(DMLOADER_MTX);
    assert(ercd == E_OK);

//    return LDMID(p_ldmcb);
    return E_OK;

error_exit:
//    free_with_acp(elt.text_mem);
//    free_with_acp(elt.data_mem);
//    if(domid == TDOM_KERNEL && p_ldmcb != NULL)
//        queue_insert_prev(&free_lkmcb, &(p_ldmcb->free_queue));
    unl_mtx(DMLOADER_MTX);
    return ercd;
}

ER dmloader_rmv_ldm(T_LDM_CAN *ldm_can) {
    // TODO: Check ldm_can status

    ER_ID ercd = E_OK;

    // Deletion
    for(SIZE i = 0; i < ldm_can->cfg_entry_num && ercd == E_OK; ++i) {
        MOD_CFG_ENTRY *ent = &ldm_can->cfg_table[i];
        switch(ent->sfncd) {
        case TSFN_CRE_TSK: {
            syslog(LOG_ERROR, "[dmloader] RMV MOD_CFG_ENTRY TSFN_CRE_TSK");
            ID tskid = *(ID*)ent->retvalptr;
            ter_tsk(tskid);
            ercd = del_tsk(tskid);
            assert(ercd == E_OK);
            break; }
        default:
            syslog(LOG_ERROR, "[dmloader] Unsupported static function code %d.", ent->sfncd);
            ercd = E_OBJ;
        }
    }

    // TODO: clean ldm_can
}

///**
// * service call
// * task context
// *
// * ER:
// * E_ID: not a user domain
// * E_OBJ: Wrong ELF data
// */
//ER_ID dmloader_ins_ldm(const uint8_t *mod_data, ID domid, intptr_t exinf) {
//    //TODO: hard coding
//    mod_data = untitled;
//
//    ER_ID ercd;
//    LDMCB *p_ldmcb = NULL;
//    elf32_load_trans elt;
//    elt.text_mem = NULL;
//    elt.data_mem = NULL;
//
//    ercd = loc_mtx(dmloader_mtx);
//    if(ercd != E_OK) {
//        syslog(LOG_ERROR, "[dmloader] Acquire mutex failed.");
//        goto error_exit;
//    }
//
//    /**
//     * Find a free LDMCB
//     * TODO:
//     * 1) check context
//     * 2) lock CPU to modify 'free_dmcb' ? => overhead too long for time tick ?
//     * 3) check domid
//     */
//    if(domid == TDOM_KERNEL) { // Loadable Kernel Module
//        if(!queue_empty(&free_lkmcb))
//            p_ldmcb = queue_delete_next(&free_lkmcb);
//    } else {                   // Loadable User Module
//        for(size_t i = 0; i < TNUM_LUMID; ++i)
//            if(lum_domid_table[i] == domid) {
//                p_ldmcb = &ldmcb_table[i];
//                if(p_ldmcb->status != LDMCB_FREE) {
//                    syslog(LOG_ERROR, "[dmloader] Dynamic user domain is already in use.");
//                    ercd = E_OBJ;
//                    goto error_exit;
//                }
//                break;
//            }
//    }
//    if(p_ldmcb == NULL) {
//        ercd = E_NOID;
//        goto error_exit;
//    }
//
//    /**
//     * ELF32 load transaction: Prepare step
//     */
//    ercd = elf32_trans_prepare(&elt, mod_data);
//    if(ercd != E_OK)
//        goto error_exit;
//
//    /**
//     * ELF32 load transaction: Allocate step
//     */
//    ACPTN tacp_domid = (domid == TDOM_KERNEL) ? TACP_KERNEL : TACP(domid);
//    elt.text_mem = malloc_with_acp(elt.text_segment->p_memsz, tacp_domid, TACP_KERNEL, tacp_domid);
//    elt.data_mem = malloc_with_acp(elt.data_segment->p_memsz, tacp_domid, tacp_domid, TACP_KERNEL);
//    if(elt.text_mem == NULL || elt.data_mem == NULL) {
//        syslog(LOG_ERROR, "[dmloader] Memory allocation for text segment or data segment failed.");
//        ercd = E_NOMEM;
//        goto error_exit;
//    }
//
//    /**
//     * ELF32 load transaction: Load step
//     */
//    ercd = elf32_trans_load(&elt);
//    if(ercd != E_OK)
//        goto error_exit;
//
//    /**
//     * Update LDMCB
//     */
//    p_ldmcb->status = LDMCB_READY;
//    p_ldmcb->text_mem = elt.text_mem;
//    p_ldmcb->data_mem = elt.data_mem;
//    p_ldmcb->entry_point = elt.entry_point;
//    p_ldmcb->exit_point = elt.exit_point;
//
//    /**
//     * Call module_initialize()
//     * TODO: call context ?
//     */
//    p_ldmcb->status = LDMCB_RUNNING;
//    p_ldmcb->entry_point(exinf);
//
//    ercd = unl_mtx(dmloader_mtx);
//    assert(ercd == E_OK);
//
//    return LDMID(p_ldmcb);
//
//error_exit:
//    free_with_acp(elt.text_mem);
//    free_with_acp(elt.data_mem);
//    if(domid == TDOM_KERNEL && p_ldmcb != NULL)
//        queue_insert_prev(&free_lkmcb, &(p_ldmcb->free_queue));
//    unl_mtx(dmloader_mtx);
//    return ercd;
//}

/**
 * service call
 * task context
 * block during call module initialize ?
 *
 * ER:
 * E_ID: not a user domain
 * E_OBJ: Wrong ELF data
 */
//ER dmloader_act_ldm(ID ldmid, intptr_t exinf) {
//    ER ercd;
//    LDMCB *p_ldmcb = NULL;
//
//    ercd = loc_mtx(dmloader_mtx);
//    if(ercd != E_OK) {
//        syslog(LOG_ERROR, "[dmloader] Acquire mutex failed.");
//        goto error_exit;
//    }
//
//    if(!VALID_LDMID(ldmid)) {
//        ercd = E_ID;
//        goto error_exit;
//    }
//
//    p_ldmcb = GET_LDMCB(ldmid);
//    if(p_ldmcb->status != LDMCB_READY) {
//        ercd = E_OBJ;
//        goto error_exit;
//    }
//
//    p_ldmcb->status = LDMCB_RUNNING;
//    p_ldmcb->entry_point(exinf);
//
//    unl_mtx(dmloader_mtx);
//
//    return E_OK;
//
//error_exit:
//    unl_mtx(dmloader_mtx);
//    return ercd;
//}

///**
// * service call
// * task context
// * block during call module initialize ?
// *
// * ER:
// * E_ID: not a user domain
// * E_OBJ: Wrong ELF data
// */
//ER dmloader_unl_ldm(ID ldmid, intptr_t exinf) {
//    ER ercd;
//    LDMCB *p_ldmcb = NULL;
//
//    ercd = loc_mtx(dmloader_mtx);
//    if(ercd != E_OK) {
//        syslog(LOG_ERROR, "[dmloader] Acquire mutex failed.");
//        goto error_exit;
//    }
//
//    if(!VALID_LDMID(ldmid)) {
//        ercd = E_ID;
//        goto error_exit;
//    }
//
//    p_ldmcb = GET_LDMCB(ldmid);
//    if(p_ldmcb->status != LDMCB_RUNNING) {
//        ercd = E_OBJ;
//        goto error_exit;
//    }
//
//    p_ldmcb->status = LDMCB_FREE;
//    p_ldmcb->exit_point(exinf);
//    // TODO: clear up for Loadable User Module
//
//    queue_insert_prev(&free_lkmcb, &(p_ldmcb->free_queue));
//
//    unl_mtx(dmloader_mtx);
//
//    return E_OK;
//
//error_exit:
//syslog(LOG_ERROR, "[dmloader] ercd: %d.", ercd); // TODO: for debug
//    unl_mtx(dmloader_mtx);
//    return ercd;
//}

//typedef struct {
//    uint8_t* mem;
//    size_t   size;
//    intptr_t addr; /* (Virtual) address */
//} load_area;
//
//typedef struct {
//    const char*    name;
//    int            type; /* enum SpecialSections */
//    const uint8_t* data;
//    size_t         size;
//    intptr_t       addr; /* (Virtual) address */
//    uint32_t       flags;
//} elf_section;

/**
 * If 'seg' is NULL, do nothing.
 */
static inline
void elf32_load_segment(void *dest, const void *elf32_data, const Elf32_Phdr *seg) {
    if(seg == NULL) return;
    memcpy(dest, elf32_data + seg->p_offset, seg->p_filesz);
    memset(dest + seg->p_filesz, '\0', seg->p_memsz - seg->p_filesz);
}

/**
 * Transaction for loading a Dynamically Loadable Module (DLM):
 * 1) Prepare step
 *    1.1) Set the pointer 'elf_data' to the memory area where the ELF file is stored.
 *    1.2) Locate text segment and data segment.
 *    1.3) Set the pointers 'text_segment' and 'data_segment' to the corresponding segment.
 * 2) Allocate step
 *    2.1) Allocate memory to load text segment and data segment.
 *    2.2) Set the pointers 'text_mem' and 'data_mem' to the corresponding memory area.
 * 3) Load step
 *    3.1) Load the contents of text segment and data segment into 'text_mem' and 'data_mem'.
 *    3.2) Perform relocations.
 *    3.3) Locate entry point and exit point.
 *    3.4) Set the pointers 'entry_point' and 'exit_point'.
 *    3.5) Call Instruction Memory Barrier (IMB).
 */

static ER elf32_trans_prepare(elf32_load_trans *elt, const uint8_t *elf32_data) {
    /**
     * Parse program headers to locate text segment and data segment
     */
    const Elf32_Ehdr *ehdr = elf32_data;

    if(ehdr->e_phoff == 0 || ehdr->e_phnum == 0) {
        syslog(LOG_ERROR, "[dmloader] ELF data doesn't have any program header.");
        return E_OBJ;
    }

    const Elf32_Phdr *seg_text = NULL;
    const Elf32_Phdr *seg_data = NULL;

    for(uint32_t i = 0; i < ehdr->e_phnum; ++i) {
        const Elf32_Phdr *phdr = ELF32_PHDR(ehdr, i);

        // Skip if not PT_LOAD segment
        if(phdr->p_type == PT_NULL) continue;
        if(phdr->p_type != PT_LOAD) {
            syslog(LOG_WARNING, "[dmloader] Unsupported segment type %d.", phdr->p_type);
            continue;
        }

        if(phdr->p_flags & PF_X) { // Text segment
            if(seg_text != NULL) {
                syslog(LOG_ERROR, "[dmloader] Only one text segment is supported.");
                return E_OBJ;
            }
            seg_text = phdr;
        } else {                   // Data segment
            if(seg_data != NULL) {
                syslog(LOG_ERROR, "[dmloader] Only one data segment is supported.");
                return E_OBJ;
            }
            seg_data = phdr;
        }
    }

    if(seg_text == NULL) {
        syslog(LOG_ERROR, "[dmloader] Text segment doesn't exist.");
        return E_OBJ;
    }

    /**
     * Perform prepare step
     */
    elt->elf_data = elf32_data;
    elt->text_segment = seg_text;
    elt->data_segment = seg_data;

    return E_OK;
}

/**
 * E_NOSPT: apply relocation failed.
 */
static ER elf32_trans_load(elf32_load_trans *elt) {
    const Elf32_Ehdr *ehdr = elt->elf_data;

    /**
     * Load the contents of text segment and data segment.
     */
    elf32_load_segment(elt->text_mem, ehdr, elt->text_segment);
    elf32_load_segment(elt->data_mem, ehdr, elt->data_segment);

    /**
     * Perform relocations and locate '.symtab' and '.strtab'
     */
    elt->symtab = NULL;
    elt->strtab = NULL;
    for(uint32_t i = 0; i < ehdr->e_shnum; ++i) {
        const Elf32_Shdr* shdr = ELF32_SHDR(ehdr, i);

        // Find '.symtab'
        if(shdr->sh_type == SHT_SYMTAB && strcmp(ELF32_SH_NAME(ehdr, shdr->sh_name), ".symtab") == 0) {
            elt->symtab = shdr;
            if(shdr->sh_link != 0) elt->strtab = ELF32_SHDR(ehdr, shdr->sh_link);
            continue;
        }

        // TODO: SHT_RELA not supported yet.
        assert(shdr->sh_type != SHT_RELA);

        if(shdr->sh_type != SHT_REL && shdr->sh_type != SHT_RELA)
            continue;

        const Elf32_Shdr* dynsym = ELF32_SHDR(ehdr, shdr->sh_link);

        for(size_t i = 0; i < shdr->sh_size / shdr->sh_entsize; ++i) {
            Elf32_Rela rela;

            if(shdr->sh_type == SHT_REL) {
                const Elf32_Rel* rel = ELF32_REL(ehdr, shdr, i);
                rela.r_info = rel->r_info;
                rela.r_offset = rel->r_offset;
                Elf32_Sword addend = *(Elf32_Sword*)elf32_vaddr_to_paddr(elt, rela.r_offset);
                rela.r_addend = addend;
            } else
                rela = *ELF32_RELA(ehdr, shdr, i);

            const Elf32_Sym* sym = ELF32_SYM(ehdr, dynsym, ELF32_R_SYM(rela.r_info));

            if(!apply_elf32_rela(elt, &rela, sym)) {
                syslog(LOG_ERROR, "[dmloader] apply_elf32_rela() failed.");
                return E_NOSPT;
            }

        }
    }
    if(elt->symtab == NULL || elt->strtab == NULL) {
        syslog(LOG_ERROR, "[dmloader] '.symtab' section doesn't exist or is corrupted.");
        return E_OBJ;
    }

    return E_OK;
}

//ER_UINT
//acre_tsk(const T_CTSK *pk_ctsk)
//{
//    TCB     *p_tcb;
//    TINIB   *p_tinib;
//    ATR     tskatr;
//    STK_T   *sstk;
//    STK_T   *stk;
//    ER      ercd;
//
//    LOG_ACRE_TSK_ENTER(pk_ctsk);
//    CHECK_TSKCTX_UNL();
//    CHECK_RSATR(pk_ctsk->tskatr, TA_ACT|TARGET_TSKATR);
//    CHECK_ALIGN_FUNC(pk_ctsk->task);
//    CHECK_NONNULL_FUNC(pk_ctsk->task);
//    CHECK_TPRI(pk_ctsk->itskpri);
//    CHECK_STKSZ_MIN(pk_ctsk->stksz);
//    CHECK_ALIGN_STKSZ(pk_ctsk->stksz);
//    CHECK_ALIGN_STACK(pk_ctsk->stk);
//    tskatr = pk_ctsk->tskatr;
//    sstk = pk_ctsk->sstk;
//    stk = pk_ctsk->stk;
//
//    t_lock_cpu();
//    if (queue_empty(&free_tcb)) {
//        ercd = E_NOID;
//    }
//    else {
//        if (sstk == NULL) {
//            sstk = malloc_with_acp(pk_ctsk->sstksz, 0, 0, 0);
//            tskatr |= TA_MEMALLOC;
//        }
//        if (stk == NULL) {
//            stk = malloc_with_acp(pk_ctsk->stksz, 0, 0, 0);
//            tskatr |= TA_MEMALLOC;
//        }
//        if (stk == NULL || sstk == NULL) {
//            ercd = E_NOMEM;
//        }
//        else {
//            p_tcb = ((TCB *) queue_delete_next(&free_tcb));
//            p_tinib = (TINIB *)(p_tcb->p_tinib);
//            p_tinib->p_dominib = &dominib_kernel; //&dominib_table[0]; // TODO: use DOM1 for debug
//            //p_tinib->p_dominib = &dominib_table[0]; // TODO: use DOM1 for debug
//            p_tinib->tskatr = tskatr;
//            p_tinib->exinf = pk_ctsk->exinf;
//            p_tinib->task = pk_ctsk->task;
//            p_tinib->ipriority = INT_PRIORITY(pk_ctsk->itskpri);
//            p_tinib->sstksz = pk_ctsk->sstksz;
//            p_tinib->sstk = sstk;
//#ifdef USE_TSKINICTXB
//            init_tskinictxb(&(p_tinib->tskinictxb), stk, pk_ctsk);
//#else /* USE_TSKINICTXB */
//            p_tinib->ustksz = pk_ctsk->stksz;
//            p_tinib->ustk = stk;
//#endif /* USE_TSKINICTXB */
//            p_tinib->texatr = TA_NULL;
//            p_tinib->texrtn = NULL;
//
//            p_tcb->actque = false;
//            queue_initialize(&(p_tcb->mutex_queue));
//            make_dormant(p_tcb);
//            if ((p_tcb->p_tinib->tskatr & TA_ACT) != 0U) {
//                make_active(p_tcb);
//            }
//            ercd = TSKID(p_tcb);
//        }
//    }
//    t_unlock_cpu();
//
//  error_exit:
//    LOG_ACRE_TSK_LEAVE(ercd);
//    return(ercd);
//}

/**
 * ER:
 * E_ID: not a user domain
 * E_OBJ: Wrong ELF data
 */
//ER dmloader_run_lkm(const uint8_t *mod_data, intptr_t exinf) {
//    //TODO: hard coding
//    mod_data = untitled;
//
//    ER ercd;
//
//    /**
//     * TODO:
//     * 1) check context
//     * 2) lock cpu to modify 'free_dmcb' ? => overhead too long for time tick ?
//     */
//     if (queue_empty(&free_dmcb)) {
//         ercd = E_NOID;
//         return ercd;
//     }
//
//    elf32_load_trans elt;
//
//    ercd = elf32_trans_prepare(&elt, mod_data);
//    if(ercd != E_OK)
//        return ercd;
//
//    /**
//     * Allocate step
//     */
//    elt.text_mem = malloc(elt.text_segment->p_memsz);
//    elt.data_mem = malloc(elt.data_segment->p_memsz);
//    if(elt.text_mem == NULL || elt.data_mem == NULL) {
//        syslog(LOG_ERROR, "[dmloader] Memory allocation for text segment or data segment failed.");
//        free(elt.text_mem);
//        free(elt.data_mem);
//        return E_NOMEM;
//    }
//
//    ercd = elf32_trans_load(&elt);
//    if(ercd != E_OK)
//        return ercd;
//
//    DMCB *p_dmcb = queue_delete_next(&free_dmcb);
//    p_dmcb->atr = TA_DOM(TDOM_KERNEL);
//    p_dmcb->text_mem = elt.text_mem;
//    p_dmcb->data_mem = elt.data_mem;
//    p_dmcb->exit_point = elt.exit_point;
//
//    INIRTN entry_point = elt.entry_point;
//
//    syslog(LOG_ERROR, "[dmloader] entry_point @ 0x%08x.", entry_point);
//
//    // TODO: call context ?
//    entry_point(exinf);
//
//    return E_OK;
//}

/**
 * TODO: not finished yet.
 * ER:
 * E_ID: not a user domain
 * E_OBJ: Wrong ELF data
 */
//ER dmloader_run_lum(const uint8_t *mod_data, ID domid, intptr_t exinf) {
//    //TODO: hard coding
//    mod_data = untitled;
//
//    ER ercd;
//
//    elf32_load_trans elt;
//
//    ercd = elf32_trans_prepare(&elt, mod_data);
//    if(ercd != E_OK)
//        return ercd;
//
//    /**
//     * Allocate step
//     */
//    elt.text_mem = malloc_with_acp(elt.text_segment->p_memsz, TACP(domid), TACP_KERNEL, TACP(domid));
//    elt.data_mem = malloc_with_acp(elt.data_segment->p_memsz, TACP(domid), TACP(domid), TACP_KERNEL);
//    if(elt.text_mem == NULL || elt.data_mem == NULL) {
//        syslog(LOG_ERROR, "[dmloader] Memory allocation for text segment or data segment failed.");
//        free_with_acp(elt.text_mem);
//        free_with_acp(elt.data_mem);
//        return E_NOMEM;
//    }
//
//    ercd = elf32_trans_load(&elt);
//    if(ercd != E_OK)
//        return ercd;
//
////    INIRTN entry_point = elt.entry_point;
//
////    syslog(LOG_ERROR, "[dmloader] entry_point @ 0x%08x.", entry_point);
//
////    assert(entry_point != NULL);
//
//    entry_point(exinf);
//
//    return E_OK;
//}

ER dmloader_del_lkm(ID modid) {
//    //TODO: hard coding
//    mod_data = untitled;
//
//    ER ercd;
//
//    elf32_load_trans elt;
//
//    ercd = elf32_trans_prepare(&elt, mod_data);
//    if(ercd != E_OK)
//        return ercd;
//
//    /**
//     * Allocate step
//     */
//    elt.text_mem = malloc_with_acp(elt.text_segment->p_memsz, TACP(domid), TACP_KERNEL, TACP(domid));
//    elt.data_mem = malloc_with_acp(elt.data_segment->p_memsz, TACP(domid), TACP(domid), TACP_KERNEL);
//    if(elt.text_mem == NULL || elt.data_mem == NULL) {
//        syslog(LOG_ERROR, "[dmloader] Memory allocation for text segment or data segment failed.");
//        free_with_acp(elt.text_mem);
//        free_with_acp(elt.data_mem);
//        return E_NOMEM;
//    }
//
//    ercd = elf32_trans_load(&elt);
//    if(ercd != E_OK)
//        return ercd;
//
//    INIRTN entry_point = elt.entry_point;
//
//    syslog(LOG_ERROR, "[dmloader] entry_point @ 0x%08x.", entry_point);
//
//    assert(entry_point != NULL);
//
//    entry_point(exinf);

    return E_OK;
}

/**
 * ER:
 * E_ID: not a user domain
 * E_OBJ: Wrong ELF data
 */
//ER dmloader_run_lum(const uint8_t *mod_data, ID domid) {
//    ER ercd;
//
//    // TODO: hard coding
//    mod_data = untitled;
////    dyn_mod_control_block dmcb;
////    dyn_mod_control_block *p_dmcb = &dmcb;
//    elf32_load_trans elt;
//    elt.elf_data = mod_data;
//
//    // TODO: check dynamic user domain && if in use
//    if(!VALID_DOMID(domid) || domid == TDOM_KERNEL) {
//        syslog(LOG_ERROR, "[dmloader] Domain %d isn't a valid dynamic user domain.", domid);
//        return E_ID;
//    }
//
//    const Elf32_Ehdr *ehdr = mod_data;
//
//    if(ehdr->e_phoff == 0 || ehdr->e_phnum == 0) {
//        syslog(LOG_ERROR, "[dmloader] Module's ELF data doesn't have any program header.");
//        return E_OBJ;
//    }
//
//    const Elf32_Phdr *seg_text = NULL;
//    const Elf32_Phdr *seg_data = NULL;
//
//    /**
//     * Parse program headers for text segment and data segment
//     */
//    for(uint32_t i = 0; i < ehdr->e_phnum; ++i) {
//        const Elf32_Phdr *phdr = ELF_PHDR(ehdr, i);
//
//        // Skip if not PT_LOAD segment
//        if(phdr->p_type == PT_NULL) continue;
//        if(phdr->p_type != PT_LOAD) {
//            syslog(LOG_WARNING, "[dmloader] Unsupported segment type %d.", phdr->p_type);
//            continue;
//        }
//
//        if(phdr->p_flags & PF_X) { // Text segment
//            if(seg_text != NULL) {
//                syslog(LOG_ERROR, "[dmloader] Only one text segment is supported.");
//                return E_OBJ;
//            }
//            seg_text = phdr;
//        } else {                   // Data segment
//            if(seg_data != NULL) {
//                syslog(LOG_ERROR, "[dmloader] Only one data segment is supported.");
//                return E_OBJ;
//            }
//            seg_data = phdr;
//        }
//    }
//
//    /**
//     * Allocate memory for text segment and data segment and
//     * load correspond data into them.
//     */
//    if(seg_text == NULL) {
//        syslog(LOG_ERROR, "[dmloader] Text segment doesn't exist.");
//        return E_OBJ;
//    }
//    elt.text_mem = malloc_with_acp(seg_text->p_memsz, TACP(domid), TACP_KERNEL, TACP(domid));
//    if(elt.text_mem == NULL) {
//        syslog(LOG_ERROR, "[dmloader] Memory allocation for text segment failed.");
//        return E_NOMEM;
//    }
//    if(seg_data != NULL) {
//        elt.data_mem = malloc_with_acp(seg_data->p_memsz, TACP(domid), TACP(domid), TACP_KERNEL);
//        if(elt.data_mem == NULL) {
//            free_with_acp(elt.text_mem);
//            syslog(LOG_ERROR, "[dmloader] Memory allocation for data segment failed.");
//            return E_NOMEM;
//        }
//    }
//    load_segment(elt.text_mem, mod_data, seg_text);
//    load_segment(elt.data_mem, mod_data, seg_data);
//    elt.text_size = seg_text->p_memsz;
//    elt.text_vaddr = seg_text->p_vaddr;
//    elt.data_size = seg_data->p_memsz;
//    elt.data_vaddr = seg_data->p_vaddr;
//
//    /**
//     * Perform relocations
//     */
//    for(uint32_t i = 0; i < ehdr->e_shnum; ++i) {
//        const Elf32_Shdr* shdr = ELF_SHDR(ehdr, i);
//
//        // TODO: SHT_RELA not supported yet.
//        assert(shdr->sh_type != SHT_RELA);
//
//        if(shdr->sh_type != SHT_REL && shdr->sh_type != SHT_RELA)
//            continue;
//
//        const Elf32_Shdr* dynsym = ELF_SHDR(ehdr, shdr->sh_link);
//
//        for(size_t i = 0; i < shdr->sh_size / shdr->sh_entsize; ++i) {
//            Elf32_Rela rela;
//
//            if(shdr->sh_type == SHT_REL) {
//                const Elf32_Rel* rel = ELF32_REL(ehdr, shdr, i);
//                rela.r_info = rel->r_info;
//                rela.r_offset = rel->r_offset;
//                Elf32_Sword addend = *(Elf32_Sword*)elf32_vaddr_to_paddr(&elt, rela.r_offset);
//                rela.r_addend = addend;
//            } else
//                rela = *ELF32_RELA(ehdr, shdr, i);
//
//            const Elf32_Sym* sym = ELF32_SYM(ehdr, dynsym, ELF32_R_SYM(rela.r_info));
//
//            if(!apply_elf32_rela(&elt, &rela, sym)) {
//                syslog(LOG_ERROR, "[dmloader] apply_elf32_rela() failed.");
//                //TODO: memory leaking
//                break;
//            }
//
//        }
//    }
//
//    dmloader_instruction_memory_barrier();
//
//    INIRTN ini = elf32_vaddr_to_paddr(&elt, ehdr->e_entry);
//
//    syslog(LOG_ERROR, "[dmloader] ini 0x%08x.", ini);
//
//    assert(ini != NULL);
//
//    ini(NULL);
//
////        T_CTSK ctsk;
////        ctsk.itskpri = TMIN_TPRI;
////        ctsk.sstk = NULL;
////        ctsk.sstksz = 4096;
////        ctsk.stk = NULL;
////        ctsk.stksz = 4096;
////        ctsk.task = la_ro.mem;
////        ctsk.tskatr = TA_ACT;
////
////        syslog(LOG_INFO, "[loader] Text start address: 0x%08x, size: %d.", text_start_address, text_end_address - text_start_address);
////        //dump_area(&la_ro);
////        TASK foo = la_ro.mem;
////
////        // Memory barrier from DDI0198D page 9-5
////        asm volatile("clean_loop:");                 /* Clean entire dcache */
////        asm volatile("mrc p15, 0, r15, c7, c10, 3");
////        asm volatile("bne clean_loop");
////        asm volatile("mcr p15, 0, r0, c7, c10, 4");  /* Drain write buffer */
////        // str rx, [ry]
////        asm volatile("mcr p15, 0, r0, c7, c5, 0");   /* Invalidate icache */
////
////
////        ER_ID tskid = acre_tsk(&ctsk);
////        tskid = act_tsk(tskid);
////        syslog(LOG_INFO, "[loader] ER_ID: %d.", tskid);
////        //foo(NULL);
////        syslog(LOG_INFO, "[loader] foo finished.");
//
//    return E_OK;
//}

/*
typedef struct {
    const uint8_t* data;
    size_t         e_shentsize;
    size_t         e_shnum;
    elf_section    shstr;
} elf_header;
*/

/*
void read_elf_header(const uint8_t *data, elf_header *hdr) {
    const Elf32_Ehdr *ehdr = data;

    hdr->data = data;
    hdr->e_shentsize = e

    hdr->shnum = ehdr->e_shnum;
    hdr->data = data;

}
*/

/*
void read_elf_section(const elf_header *hdr, uint32_t idx) {

}
*/

/*
bool_t elf_get_section(const Elf32_Ehdr *ehdr, uint32_t idx, elf_section* sec) {
    assert(idx < ehdr->e_shnum);
    const Elf32_Shdr* shdr = (const uint8_t *)ehdr + ehdr->e_shoff + ehdr->e_shentsize * idx;

    return (const Elf32_Shdr*)((const uint8_t *)ehdr + ehdr->e_shoff + ehdr->e_shentsize * idx);
}
*/

//static void read_section(const Elf32_Ehdr *ehdr, uint32_t idx, elf_section *sec) {
//    const Elf32_Shdr* shdr = ELF_SHDR(ehdr, idx);
//
//    sec->name  = ELF_SH_NAME(ehdr, shdr->sh_name);
//    sec->data  = ((uint8_t*)ehdr) + shdr->sh_offset;
//    sec->size  = shdr->sh_size;
//    sec->addr  = shdr->sh_addr;
//    sec->flags = shdr->sh_flags;
//
//    const char* const section_names[] = {".bss",      ".data",      ".got",      ".rodata",      ".text"};
//    const int         section_types[] = {SECTION_BSS, SECTION_DATA, SECTION_GOT, SECTION_RODATA, SECTION_TEXT};
//
//    sec->type = SECTION_UNKNOWN;
//    for(size_t i = 0; i < sizeof(section_names) / sizeof(section_names[0]); ++i)
//        if(strcmp(sec->name, section_names[i]) == 0) {
//            sec->type = section_types[i];
//            break;
//        }
//
//}

/**
 * When sec == NULL, do nothing
 */
//static bool_t load_section(const load_area *area, const elf_section *sec) {
//
//    if(sec == NULL) return true;
//
//    if(sec->addr < area->addr || sec->addr + sec->size > area->addr + area->size) {
//        syslog(LOG_ERROR, "[loader] Section %s [%08x,%08x] can't be loaded into area [%08x,%08x].", sec->name, sec->addr, sec->addr + sec->size, area->addr, area->addr + area->size);
//        return false;
//    }
//
//    for(size_t i = 0; i < sec->size; ++i) {
//        if(sec->type == SECTION_BSS)
//            area->mem[sec->addr - area->addr + i] = 0;
//        else
//            area->mem[sec->addr - area->addr + i] = sec->data[i];
//    }
//
//    return true;
//}

#define LOG_INFO LOG_EMERG

//void dump_area(const load_area *area) {
//    for(size_t i = 0; i < area->size; i+= 4) {
//        syslog(LOG_EMERG, "0x%08x: %02x %02x %02x %02x", area->addr + i, area->mem[i+3], area->mem[i+2], area->mem[i+1], area->mem[i]);
//    }
//}

//void load_elf(const uint8_t *data, uint32_t domid) {
//    data = untitled;
//
//    const Elf32_Ehdr *ehdr = data;
//    elf_section sections[ehdr->e_shnum];
//
//    const elf_section *sec_text   = NULL;
//    const elf_section *sec_rodata = NULL;
//    const elf_section *sec_got    = NULL;
//    const elf_section *sec_data   = NULL;
//    const elf_section *sec_bss    = NULL;
//
//    /* Virtual addresses */
//    uintptr_t text_start_address = UINT32_MAX;
//    uintptr_t text_end_address   = 0;
//    uintptr_t data_start_address = UINT32_MAX;
//    uintptr_t data_end_address   = 0;
//
//    for(uint32_t i = 0; i < ehdr->e_shnum; ++i) {
//        read_section(ehdr, i, &sections[i]);
//
//        syslog(LOG_INFO, "Section[%d]:", i);
//        syslog(LOG_INFO, "  Name: %s", sections[i].name);
//        syslog(LOG_INFO, "  Size: %d", sections[i].size);
//
//        switch(sections[i].type) {
//        case SECTION_BSS:
//            sec_bss = &sections[i];
//            break;
//
//        case SECTION_DATA:
//            sec_data = &sections[i];
//            break;
//
//        case SECTION_GOT:
//            sec_got = &sections[i];
//            break;
//
//        case SECTION_RODATA:
//            sec_rodata = &sections[i];
//            break;
//
//        case SECTION_TEXT:
//            sec_text = &sections[i];
//            break;
//        }
//
//        switch(sections[i].type) {
//        /* Data sections */
//        case SECTION_BSS:
//        case SECTION_DATA:
//            if(data_start_address > sections[i].addr)
//                data_start_address = sections[i].addr;
//            if(data_end_address < sections[i].addr + sections[i].size)
//                data_end_address = sections[i].addr + sections[i].size;
//            break;
//
//        /* Text sections */
//        case SECTION_GOT:
//        case SECTION_RODATA:
//        case SECTION_TEXT:
//            if(text_start_address > sections[i].addr)
//                text_start_address = sections[i].addr;
//            if(text_end_address < sections[i].addr + sections[i].size)
//                text_end_address = sections[i].addr + sections[i].size;
//            break;
//        }
//    }
//
//    if(sec_text == NULL) {
//        syslog(LOG_ERROR, "[loader] Error: .text section not found.");
//        return;
//    }
//
//    // Construct load_area for read only sections (.text, .rodata & .got)
//    load_area la_rw;
//    la_rw.addr = data_start_address;
//    la_rw.size = data_end_address - data_start_address;
//    la_rw.mem = malloc_with_acp(la_rw.size, 1 << domid, 1 << domid, 0);
//    load_section(&la_rw, sec_bss);
//    load_section(&la_rw, sec_data);
//    syslog(LOG_INFO, "[loader] Data loaded @ 0x%08x.", (uintptr_t)la_rw.mem);
//
//    // Construct load_area for read only sections (.text, .rodata & .got)
//    load_area la_ro;
//    la_ro.addr = text_start_address;
//    la_ro.size = text_end_address - text_start_address;
//    la_ro.mem = malloc_with_acp(la_ro.size, 1 << domid, 0, 1 << domid);
//    load_section(&la_ro, sec_text);
//    load_section(&la_ro, sec_rodata);
//    load_section(&la_ro, sec_got);
//    if(sec_got != NULL) {
//    	uint32_t *got = la_ro.mem + sec_got->addr - la_ro.addr;
//
//    	size_t data_num = sec_got->size / sizeof(uint32_t);
//    	uintptr_t offset = (uintptr_t)la_rw.mem - (uintptr_t)data_start_address;
//    	assert((uintptr_t)la_rw.mem >= (uintptr_t)data_start_address);
//
//    	for(size_t i = 0; i < sec_got->size / sizeof(uint32_t); ++i) {
//    	    //assert(got[i] >= data_start_address); TODO: Zero address mapping
//    		syslog(LOG_INFO, "[loader] GOT: 0x%08x => 0x%08x.", got[i], got[i] + offset);
//    		got[i] += offset;
//    	}
//    }
//    syslog(LOG_INFO, "[loader] Text loaded @ 0x%08x.", (uintptr_t)la_ro.mem);
//
//    T_CTSK ctsk;
//    ctsk.itskpri = TMIN_TPRI;
//    ctsk.sstk = NULL;
//    ctsk.sstksz = 4096;
//    ctsk.stk = NULL;
//    ctsk.stksz = 4096;
//    ctsk.task = la_ro.mem;
//    ctsk.tskatr = TA_ACT;
//
//    syslog(LOG_INFO, "[loader] Text start address: 0x%08x, size: %d.", text_start_address, text_end_address - text_start_address);
//    //dump_area(&la_ro);
//    TASK foo = la_ro.mem;
//
//    // Memory barrier from DDI0198D page 9-5
//    asm volatile("clean_loop:");                 /* Clean entire dcache */
//    asm volatile("mrc p15, 0, r15, c7, c10, 3");
//    asm volatile("bne clean_loop");
//    asm volatile("mcr p15, 0, r0, c7, c10, 4");  /* Drain write buffer */
//    // str rx, [ry]
//    asm volatile("mcr p15, 0, r0, c7, c5, 0");   /* Invalidate icache */
//
//
//    ER_ID tskid = acre_tsk(&ctsk);
//    tskid = act_tsk(tskid);
//    syslog(LOG_INFO, "[loader] ER_ID: %d.", tskid);
//    //foo(NULL);
//    syslog(LOG_INFO, "[loader] foo finished.");
//
////    size_t ro_size;
////    const elf_section *last_ro_sec = sec_text;
////    if(got_)
//
//    // check text + rodata size
//
//
//    // check if got exists
//}

/**
 * ER:
 * E_ID: not a user domain
 * E_OBJ: Wrong ELF data
 */
//ER dmloader_run_lum(const uint8_t *mod_data, ID domid) {
//    ER ercd;
//
//    // TODO: hard coding
//    mod_data = untitled;
////    dyn_mod_control_block dmcb;
////    dyn_mod_control_block *p_dmcb = &dmcb;
//    elf32_load_trans elt;
//    elt.elf_data = mod_data;
//
//    // TODO: check dynamic user domain && if in use
//    if(!VALID_DOMID(domid) || domid == TDOM_KERNEL) {
//        syslog(LOG_ERROR, "[dmloader] Domain %d isn't a valid dynamic user domain.", domid);
//        return E_ID;
//    }
//
//    const Elf32_Ehdr *ehdr = mod_data;
//
//    if(ehdr->e_phoff == 0 || ehdr->e_phnum == 0) {
//        syslog(LOG_ERROR, "[dmloader] Module's ELF data doesn't have any program header.");
//        return E_OBJ;
//    }
//
//    const Elf32_Phdr *seg_text = NULL;
//    const Elf32_Phdr *seg_data = NULL;
//
//    /**
//     * Parse program headers for text segment and data segment
//     */
//    for(uint32_t i = 0; i < ehdr->e_phnum; ++i) {
//        const Elf32_Phdr *phdr = ELF_PHDR(ehdr, i);
//
//        // Skip if not PT_LOAD segment
//        if(phdr->p_type == PT_NULL) continue;
//        if(phdr->p_type != PT_LOAD) {
//            syslog(LOG_WARNING, "[dmloader] Unsupported segment type %d.", phdr->p_type);
//            continue;
//        }
//
//        if(phdr->p_flags & PF_X) { // Text segment
//            if(seg_text != NULL) {
//                syslog(LOG_ERROR, "[dmloader] Only one text segment is supported.");
//                return E_OBJ;
//            }
//            seg_text = phdr;
//        } else {                   // Data segment
//            if(seg_data != NULL) {
//                syslog(LOG_ERROR, "[dmloader] Only one data segment is supported.");
//                return E_OBJ;
//            }
//            seg_data = phdr;
//        }
//    }
//
//    /**
//     * Allocate memory for text segment and data segment and
//     * load correspond data into them.
//     */
//    if(seg_text == NULL) {
//        syslog(LOG_ERROR, "[dmloader] Text segment doesn't exist.");
//        return E_OBJ;
//    }
//    elt.text_mem = malloc_with_acp(seg_text->p_memsz, TACP(domid), TACP_KERNEL, TACP(domid));
//    if(elt.text_mem == NULL) {
//        syslog(LOG_ERROR, "[dmloader] Memory allocation for text segment failed.");
//        return E_NOMEM;
//    }
//    if(seg_data != NULL) {
//        elt.data_mem = malloc_with_acp(seg_data->p_memsz, TACP(domid), TACP(domid), TACP_KERNEL);
//        if(elt.data_mem == NULL) {
//            free_with_acp(elt.text_mem);
//            syslog(LOG_ERROR, "[dmloader] Memory allocation for data segment failed.");
//            return E_NOMEM;
//        }
//    }
//    load_segment(elt.text_mem, mod_data, seg_text);
//    load_segment(elt.data_mem, mod_data, seg_data);
//    elt.text_size = seg_text->p_memsz;
//    elt.text_vaddr = seg_text->p_vaddr;
//    elt.data_size = seg_data->p_memsz;
//    elt.data_vaddr = seg_data->p_vaddr;
//
//    /**
//     * Perform relocations
//     */
//    for(uint32_t i = 0; i < ehdr->e_shnum; ++i) {
//        const Elf32_Shdr* shdr = ELF_SHDR(ehdr, i);
//
//        // TODO: SHT_RELA not supported yet.
//        assert(shdr->sh_type != SHT_RELA);
//
//        if(shdr->sh_type != SHT_REL && shdr->sh_type != SHT_RELA)
//            continue;
//
//        const Elf32_Shdr* dynsym = ELF_SHDR(ehdr, shdr->sh_link);
//
//        for(size_t i = 0; i < shdr->sh_size / shdr->sh_entsize; ++i) {
//            Elf32_Rela rela;
//
//            if(shdr->sh_type == SHT_REL) {
//                const Elf32_Rel* rel = ELF32_REL(ehdr, shdr, i);
//                rela.r_info = rel->r_info;
//                rela.r_offset = rel->r_offset;
//                Elf32_Sword addend = *(Elf32_Sword*)elf32_vaddr_to_paddr(&elt, rela.r_offset);
//                rela.r_addend = addend;
//            } else
//                rela = *ELF32_RELA(ehdr, shdr, i);
//
//            const Elf32_Sym* sym = ELF32_SYM(ehdr, dynsym, ELF32_R_SYM(rela.r_info));
//
//            if(!apply_elf32_rela(&elt, &rela, sym)) {
//                syslog(LOG_ERROR, "[dmloader] apply_elf32_rela() failed.");
//                //TODO: memory leaking
//                break;
//            }
//
//        }
//    }
//
//    dmloader_instruction_memory_barrier();
//
//    INIRTN ini = elf32_vaddr_to_paddr(&elt, ehdr->e_entry);
//
//    syslog(LOG_ERROR, "[dmloader] ini 0x%08x.", ini);
//
//    assert(ini != NULL);
//
//    ini(NULL);
//
////        T_CTSK ctsk;
////        ctsk.itskpri = TMIN_TPRI;
////        ctsk.sstk = NULL;
////        ctsk.sstksz = 4096;
////        ctsk.stk = NULL;
////        ctsk.stksz = 4096;
////        ctsk.task = la_ro.mem;
////        ctsk.tskatr = TA_ACT;
////
////        syslog(LOG_INFO, "[loader] Text start address: 0x%08x, size: %d.", text_start_address, text_end_address - text_start_address);
////        //dump_area(&la_ro);
////        TASK foo = la_ro.mem;
////
////        // Memory barrier from DDI0198D page 9-5
////        asm volatile("clean_loop:");                 /* Clean entire dcache */
////        asm volatile("mrc p15, 0, r15, c7, c10, 3");
////        asm volatile("bne clean_loop");
////        asm volatile("mcr p15, 0, r0, c7, c10, 4");  /* Drain write buffer */
////        // str rx, [ry]
////        asm volatile("mcr p15, 0, r0, c7, c5, 0");   /* Invalidate icache */
////
////
////        ER_ID tskid = acre_tsk(&ctsk);
////        tskid = act_tsk(tskid);
////        syslog(LOG_INFO, "[loader] ER_ID: %d.", tskid);
////        //foo(NULL);
////        syslog(LOG_INFO, "[loader] foo finished.");
//
//    return E_OK;
//}
