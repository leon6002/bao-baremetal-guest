/** 
 * Bao, a Lightweight Static Partitioning Hypervisor 
 *
 * Copyright (c) Bao Project (www.bao-project.org), 2019-
 *
 * Authors:
 *      Jose Martins <jose.martins@bao-project.org>
 *      Sandro Pinto <sandro.pinto@bao-project.org>
 *
 * Bao is free software; you can redistribute it and/or modify it under the
 * terms of the GNU General Public License version 2 as published by the Free
 * Software Foundation, with a special exception exempting guest code from such
 * license. See the COPYING file in the top-level directory for details. 
 *
 */

#include <core.h>
#include <page_tables.h>
#include <plat.h>

.section .page_tables, "aw"

.balign (16 * 0x400)
.global page_table
page_table:
    .set ADDR, 0

    /* Map from 0 to MEM_BASE as device */
    .set COUNT, MEM_BLK_IDX
    .rept   COUNT
    .4byte (PTE_SUPERPAGE | PTE_DEV_FLAGS) + ADDR
    .set ADDR, ADDR + L1_BLOCK_SIZE
    .endr

    /* Map the 1MB memory region (MEM_BASE to MEM_BASE+1MB) as memory */
    .4byte (PTE_SUPERPAGE | PTE_MEM_FLAGS) + ADDR
    .set ADDR, ADDR + L1_BLOCK_SIZE

    /* Map the rest (ADDR to 4GB) as device */
    /* Total 4096 blocks. Used: MEM_BLK_IDX + 1 */
    .set COUNT, 4096 - (MEM_BLK_IDX + 1)
    .rept   COUNT
    .4byte (PTE_SUPERPAGE | PTE_DEV_FLAGS) + ADDR
    .set ADDR, ADDR + L1_BLOCK_SIZE
    .endr
    
