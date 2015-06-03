/*
 * elf32-arm.c
 *
 *  Created on: Dec 11, 2013
 *      Author: liyixiao
 */

/**
 * ELF for the ARM® Architecture
 * ARM IHI 0044E, current through ABI release 2.09
 */

#include <kernel.h>
#include <t_syslog.h>
#include "dmloader_impl.h"

//#define DEBUG
#define LOG_DEBUG LOG_ERROR

enum RelocationCode {
    R_ARM_ABS32     = 2,
    R_ARM_GLOB_DAT  = 21,
    R_ARM_JUMP_SLOT = 22,
    R_ARM_RELATIVE  = 23,
};

/**
 * dest: position to apply relocation
 * symbol: physical address of the symbol in ELF32_R_SYM
 * type: ELF32_R_TYPE
 * addend
 */
bool_t apply_elf32_rela(elf32_load_trans *elt, const Elf32_Rela *rela, const Elf32_Sym *sym) {
    Elf32_Addr *dest = elf32_vaddr_to_paddr(elt, rela->r_offset);

    if(dest == NULL) {
        syslog(LOG_ERROR, "[dmloader] Invalid virtual address for relocation.");
        return false;
    }

    /**
     * By now, only R_ARM_RELATIVE and R_ARM_ABS32 relocations are supported.
     */
    switch(ELF32_R_TYPE(rela->r_info)) {
    case R_ARM_RELATIVE:
        // TODO: Thumb instruction not supported yet.
        *dest = (Elf32_Addr)elf32_vaddr_to_paddr(elt, rela->r_addend);
#ifdef DEBUG
        syslog(LOG_DEBUG, "[dmloader] Relocation: 0x%x(p:0x%08x) => 0x%08x.", rela->r_offset, dest, *dest);
#endif
        return true;
        break;
    case R_ARM_ABS32:
        assert(sym != NULL);
    	*dest = (Elf32_Addr)elf32_vaddr_to_paddr(elt, sym->st_value + rela->r_addend);
#ifdef DEBUG
        syslog(LOG_DEBUG, "[dmloader] Relocation: 0x%x(p:0x%08x) => 0x%08x.", rela->r_offset, dest, *dest);
#endif
    	return true;
    	break;
    }

    syslog(LOG_ERROR, "[dmloader] Unsupported relocation info %x.", rela->r_info);
    return false;
}

void dmloader_instruction_memory_barrier() {
    // Memory barrier from DDI0198D page 9-5
    asm volatile("clean_loop:");                 /* Clean entire dcache */
    asm volatile("mrc p15, 0, r15, c7, c10, 3");
    asm volatile("bne clean_loop");
    asm volatile("mcr p15, 0, r0, c7, c10, 4");  /* Drain write buffer */
    // TODO: str rx, [ry]
    asm volatile("mcr p15, 0, r0, c7, c5, 0");   /* Invalidate icache */
}
