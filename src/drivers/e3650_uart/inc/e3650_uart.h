/*
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#ifndef __E3650_UART_H__
#define __E3650_UART_H__

#include <stdint.h>

struct e3650_uart {
    uint32_t rbr_thr_dll;
    uint32_t dlh_ier;
    uint32_t iir_fcr;
    uint32_t lcr;
    uint32_t mcr;
    uint32_t lsr;
    uint32_t msr;
    uint32_t scr;
};

extern volatile struct e3650_uart uart;

/* Driver implements the generic interface defined in core/inc/uart.h */
void uart_init(void);
void uart_putc(char c);

/* Stubs for unused functions in this minimal port */
char uart_getchar(void);
void uart_enable_rxirq(void);
void uart_clear_rxirq(void);

#endif /* __E3650_UART_H__ */
