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
#include <stdlib.h>
#include <stdio.h>
#include <cpu.h>
#include <wfi.h>
#include <spinlock.h>
#include <plat.h>
#include <irq.h>
#include <uart.h>
#include <timer.h>

#define TIMER_INTERVAL (TIME_S(1))

spinlock_t print_lock = SPINLOCK_INITVAL;

void main(void);

void uart_rx_handler(unsigned id){
    (void)id;
    int vm_id = ((unsigned long)&main >= 0x00C00000) ? 1 : 0;
    printf("[VM%d] cpu%d: %s\n", vm_id, get_cpuid(), __func__);
    uart_clear_rxirq();
}

void ipi_handler(unsigned id){
    (void)id;
    int vm_id = ((unsigned long)&main >= 0x00C00000) ? 1 : 0;
    printf("[VM%d] cpu%d: %s\n", vm_id, get_cpuid(), __func__);
    irq_send_ipi(1ull << (get_cpuid() + 1));
}

void timer_handler(unsigned id){
    (void)id;
    int vm_id = ((unsigned long)&main >= 0x00C00000) ? 1 : 0;
    printf("[VM%d] cpu%d: %s\n", vm_id, get_cpuid(), __func__);
    timer_set(TIMER_INTERVAL);
    irq_send_ipi(1ull << (get_cpuid() + 1));
}

void main(void){

    /* Identify VM based on build time MEM_BASE or runtime check */
    unsigned long my_addr = (unsigned long)&main;
    int vm_id = (my_addr >= 0x00C00000) ? 1 : 0;

    /* Extra huge delay for VM1 at startup to avoid banner collision */
    if (vm_id == 1) {
        for(volatile int k = 0; k < 40000000; k++); 
    }

    static volatile bool master_done = false;

    if(cpu_is_master()){
        spin_lock(&print_lock);
        /* Identify VM again for banner */
        printf("[VM%d] Bao bare-metal test guest\n", vm_id);
        spin_unlock(&print_lock);

        irq_set_handler(UART_IRQ_ID, uart_rx_handler);
        irq_set_handler(TIMER_IRQ_ID, timer_handler);
        irq_set_handler(IPI_IRQ_ID, ipi_handler);

        uart_enable_rxirq();

        timer_set(TIMER_INTERVAL);
        irq_enable(TIMER_IRQ_ID);
        irq_set_prio(TIMER_IRQ_ID, TIMER_IRQ_PRIO);

        master_done = true;
    }

    irq_enable(UART_IRQ_ID);
    irq_set_prio(UART_IRQ_ID, UART_IRQ_PRIO);
    irq_enable(IPI_IRQ_ID);
    irq_set_prio(IPI_IRQ_ID, UART_IRQ_PRIO);

    while(!master_done);

    /* spin wait to avoid uart overwrites */
    for(volatile int i = 0; i < get_cpuid() * 200000; i++);

    spin_lock(&print_lock);
    printf("[VM%d] cpu %d up\n", vm_id, get_cpuid());
    spin_unlock(&print_lock);

    while(1) wfi();
}
