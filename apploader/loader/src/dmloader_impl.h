/*
 * dmloader_impl.h
 *
 *  Created on: Dec 12, 2013
 *      Author: liyixiao
 */

#pragma once

#include <queue.h>
#include "elf.h"

typedef struct {
//    ID               domid;
    const uint8_t*    elf_data; // TODO: rename to elf32_data ?
    const Elf32_Phdr* text_segment;
    const Elf32_Phdr* data_segment;
//    void*             entry_point;
//    void*             exit_point;
    uint8_t*          text_mem;
    uint8_t*          data_mem;
    const Elf32_Shdr* symtab;
    const Elf32_Shdr* strtab;
//    Elf32_Addr    text_vaddr;
//    Elf32_Word    text_size;
//    Elf32_Word    data_size;
//    Elf32_Addr    data_vaddr;
} elf32_load_trans;

static inline
void *elf32_vaddr_to_paddr(elf32_load_trans *elt, Elf32_Addr vaddr) {
    if(vaddr >= elt->text_segment->p_vaddr && vaddr < elt->text_segment->p_vaddr + elt->text_segment->p_memsz)
        return elt->text_mem + (vaddr - elt->text_segment->p_vaddr);
    if(vaddr >= elt->data_segment->p_vaddr && vaddr < elt->data_segment->p_vaddr + elt->data_segment->p_memsz)
        return elt->data_mem + (vaddr - elt->data_segment->p_vaddr);
//    if(vaddr >= elt->data_vaddr && vaddr < elt->data_vaddr + elt->data_size)
//        return elt->data_mem + (vaddr - elt->data_vaddr);
    return NULL;
}

//typedef struct {
//    uint8_t *text_mem;
//    uint8_t *data_mem;
//    TERRTN  exit_point;
//    ATR     atr;
//    QUEUE   free_dmcb_queue;
//} DMCB; //dyn_mod_control_block;

typedef union {
    ID     domid;      // For Loadable User Module (LUM)
    QUEUE  free_queue; // For Loadable Kernel Module (LKM)
} LDMINFO;

typedef struct {
    uint8_t *text_mem;
    uint8_t *data_mem;
    TERRTN  exit_point;
    ATR     dlmatr;
//    DLMINFO dlminf;
} DLMCB; //dyn_mod_control_block;

enum ldmcb_status {
    LDMCB_FREE,
    LDMCB_READY,
    LDMCB_RUNNING,
};

typedef struct {
//    bool_t  in_use;
//    LDMINFO ldminfo; // QUEUE entry must be first.
    QUEUE   free_queue; // For Loadable Kernel Module (LKM)
    int8_t  status;
    uint8_t *text_mem;
    uint8_t *data_mem;
    INIRTN  entry_point;
    TERRTN  exit_point;
//    QUEUE   free_lkmcb_queue; // For Loadable Kernel Module (LKM)
} LDMCB; //dyn_mod_control_block;

//typedef struct {
//    uint8_t *text_mem;
//    uint8_t *data_mem;
//    TERRTN  exit_point;
//    ATR     atr;
//    QUEUE   free_dmcb_queue;
//} DMCB; //dyn_mod_control_block;

extern bool_t apply_elf32_rela(elf32_load_trans *elt, const Elf32_Rela *rela, const Elf32_Sym *sym);
extern void dmloader_instruction_memory_barrier();
