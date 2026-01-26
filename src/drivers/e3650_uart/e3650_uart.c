#include <e3650_uart.h>
#include <fences.h>


/* UNUSED_ARG macro commonly used in Bao drivers */
#ifndef UNUSED_ARG
#define UNUSED_ARG(opt) ((void)opt)
#endif

void e3650_uart_init(volatile struct e3650_uart* uart)
{
    /* 0. Check if already enabled (Preserve Loader/Bao Config) */
    if (uart->mcr0 & MCR0_MODEN) {
        return;
    }

    /* 1. Disable Module */
    uart->mcr0 = 0;

    /* 2. Reset Module */
    uart->mcr0 |= MCR0_MODRST; 
    while (uart->mcr0 & MCR0_MODRST) { 
        /* Wait for self-clearing reset */
    }

    /* 3. Configure Mode & Data Frame */
    /* Defaults: Serial Mode, Full Duplex, 8N1 */
    uart->pcr0 = 0;

    /* 4. Clear FIFOs */
    /* Write 1 to Bit 16 to clear TX/RX FIFOs */
    uart->fcr0 = 0; 
    uart->fcr1 = 0; 
    uart->fcr0 |= FCR_FIFO_CLR;
    uart->fcr1 |= FCR_FIFO_CLR;
    
    /* 5. Configure Filters */
    /* RXSYNCEN (Bit 17) | RXFILTCTL=3 (Bits 18-20) */
    uint32_t filter_cfg = PCR8_RXSYNCEN | PCR8_RXFILTCTL_VAL;
    uart->pcr8  = filter_cfg;
    uart->pcr10 = filter_cfg;

    /* 6. Set Baud Rate */
    uart->pcr1 = UART_BAUDRATE_DIV;

    /* 7. Enable TX/RX */
    uart->pcr0 |= (PCR0_TXEN | PCR0_RXEN);

    /* 8. Disable Debug Mode configuration */
    uart->mcr0 |= MCR0_DEBUG_CTRL;
    uart->mcr0 &= ~MCR0_DEBUG_SWEN;

    /* 9. enable */
    uart->mcr0 |= MCR0_MODEN;


}

void e3650_uart_enable(volatile struct e3650_uart* uart)
{
    UNUSED_ARG(uart);
    return;
}

void e3650_uart_putc(volatile struct e3650_uart* uart, int8_t c)
{
    /* Startup Delay for VM1 (High Address) to avoid log interleaving */
    static int started = 0;
    if (!started) {
        /* Check if we are running in the second VM (loaded > 0xB80000) */
        /* We use the address of this function as a reference */
        unsigned long my_addr = (unsigned long)e3650_uart_putc;
        if (my_addr > 0xB80000) { 
             /* Spin for significant time to let VM0 finish banner */
             for(volatile int i=0; i<40000000; i++);
        }
        started = 1;
    }

    /* Wait until TX FIFO is not full */
    while (uart->fsr0 & FSR0_TX_FULL) { }

    /* Write character to TX Data Register */
    uart->txdr = (uint32_t)c;
}

void e3650_uart_puts(volatile struct e3650_uart* uart, const int8_t* str)
{
    while (*str) {
        e3650_uart_putc(uart, *str++);
    }
}
