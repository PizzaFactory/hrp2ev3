/*
 * dmloader.h
 *
 *  Created on: Dec 12, 2013
 *      Author: liyixiao
 */

#pragma once

#include "../app/common/module_common.h"

typedef struct {
    ID       domid;
    uint8_t  *text_mem;
    uint32_t text_vaddr;
    uint8_t  *data_mem;
    uint32_t data_vaddr;
} dyn_mod_control_block;

typedef struct {
    ID     domid;
    void*  text_mempool;
    SIZE   text_mempool_size;
    void*  data_mempool;
    SIZE   data_mempool_size;
    int8_t status; // LDM_CAN_FREE, LDM_CAN_RUNNING
// Following only available when status == LDM_CAN_RUNNING
    MOD_CFG_ENTRY* cfg_table;
    SIZE           cfg_entry_num;
} T_LDM_CAN; // Loadable Module Container

extern void dmloader_initialize(intptr_t);
