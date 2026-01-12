/*
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <e3650_uart.h>

#define UART_LSR_THRE    (1 << 5)

void uart_init(void)
{
    /* Use global uart instance */
    (void)uart;
}

void uart_putc(char c)
{
    while (!(uart.lsr & UART_LSR_THRE))
        ;

    uart.rbr_thr_dll = c;
}

char uart_getchar(void) { return 0; }
void uart_enable_rxirq(void) {}
void uart_clear_rxirq(void) {}
