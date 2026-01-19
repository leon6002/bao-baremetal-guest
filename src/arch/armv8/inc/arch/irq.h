#ifndef ARCH_IRQ_H
#define ARCH_IRQ_H

#ifndef IPI_IRQ_ID
#define IPI_IRQ_ID (0)
#endif
#ifndef TIMER_IRQ_ID
#define TIMER_IRQ_ID (27)
#endif
#define IRQ_NUM (1024)
#define IRQ_MAX_PRIO (0)

#ifndef UART_IRQ_PRIO
#define UART_IRQ_PRIO   IRQ_MAX_PRIO
#endif
#ifndef TIMER_IRQ_PRIO
#define TIMER_IRQ_PRIO  IRQ_MAX_PRIO
#endif
#ifndef IPI_IRQ_PRIO
#define IPI_IRQ_PRIO    IRQ_MAX_PRIO
#endif

#endif /* ARCH_IRQ_H */
