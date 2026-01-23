/*
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#ifndef __PLAT_H__
#define __PLAT_H__

/* 
 * Memory Map
 * Based on e3650 config: SRAM at 0x00A00000 
 * UART at 0xF8D60000
 */
#undef MEM_BASE
#define MEM_BASE        0x00B00000
#undef MEM_SIZE
#define MEM_SIZE        0x00100000

#undef UART_BASE
#define UART_BASE       0xF8D60000

/* GIC Base Addresses (Overriding defaults in gic.h) */
#define PLAT_GICD_BASE_ADDR (0xF4000000)
#define PLAT_GICC_BASE_ADDR (0xF4200000)
#define PLAT_GICR_BASE_ADDR (0xF4100000)

#undef UART_IRQ_ID
#define UART_IRQ_ID     0 
#undef UART_IRQ_PRIO
#define UART_IRQ_PRIO   1
#undef TIMER_IRQ_ID
#define TIMER_IRQ_ID    27
#undef TIMER_IRQ_PRIO
#define TIMER_IRQ_PRIO  1
#undef IPI_IRQ_ID
#define IPI_IRQ_ID      13

#ifndef __ASSEMBLER__

#include <e3650_uart.h>

extern volatile struct e3650_uart uart;

#endif /* __ASSEMBLER__ */

#endif /* __PLAT_H__ */
