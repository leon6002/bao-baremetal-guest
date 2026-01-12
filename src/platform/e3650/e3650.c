/*
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <e3650_uart.h>
#include <plat.h>

/* Global uart structure definition */
volatile struct e3650_uart uart __attribute__((section(".uart_reg")));

void platform_init(void)
{
    uart_init();
}
