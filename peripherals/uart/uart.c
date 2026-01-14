#include "uart.h"
#include "uart_rx_buf.h"
#include "../../event.h"

#define UART0_BASE 0x4000C000

#define UART_DR    (*(volatile uint32_t *)(UART0_BASE + 0x000))
#define UART_FR    (*(volatile uint32_t *)(UART0_BASE + 0x018))
#define UART_IBRD  (*(volatile uint32_t *)(UART0_BASE + 0x024))
#define UART_FBRD  (*(volatile uint32_t *)(UART0_BASE + 0x028))
#define UART_LCRH  (*(volatile uint32_t *)(UART0_BASE + 0x02C))
#define UART_CTL   (*(volatile uint32_t *)(UART0_BASE + 0x030))
#define UART_IM    (*(volatile uint32_t *)(UART0_BASE + 0x038))
#define UART_ICR   (*(volatile uint32_t *)(UART0_BASE + 0x044))

#define SYSCTL_RCGCUART (*(volatile uint32_t *)0x400FE104)
#define NVIC_EN0        (*(volatile uint32_t *)0xE000E100)

#define UART0_IRQ 5

void uart_init(void) {
    // Enable UART0 clock
    SYSCTL_RCGCUART |= (1 << 0);

    // Disable UART during config
    UART_CTL = 0x0;

    // Baud rate (arbitrary but valid for QEMU)
    UART_IBRD = 8;
    UART_FBRD = 44;

    // 8-bit, no parity, 1 stop bit, FIFO enabled
    UART_LCRH = (0x3 << 5) | (1 << 4);

    // Enable RX interrupt
    UART_IM |= (1 << 4); // RXIM

    // Enable UART, RX, TX
    UART_CTL = (1 << 0) | (1 << 8) | (1 << 9);

    // Enable IRQ in NVIC
    NVIC_EN0 |= (1 << UART0_IRQ);
}

void UART0_Handler(void) {
    while (!(UART_FR & (1 << 4))) {
        uint8_t byte = UART_DR & 0xFF;
        uart_rx_buffer_put(byte);
    }

    /* Signal main ONCE per interrupt */
    event_post_from_isr(EVENT_UART_RX, 0);

    UART_ICR |= (1 << 4);
}