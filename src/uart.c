#include "uart.h"

#define UART_REG_WRITE(reg, val) (*reg = val)
#define UART_REG_READ(reg) (*reg)

void uart_init()
{
    // Set word length
    UART_REG_WRITE(UART_LCR, 0x03);

    // Enable FIFO
    UART_REG_WRITE(UART_FCR, 0x01);

    // Enable rx buffer interrupts
    UART_REG_WRITE(UART_IER, 0x01);

}

int uart_putc(char c) {
	while ((*UART_LSR & UART_LSR_EMPTY_MASK) == 0)
        ;
        
	return *UART_THR = c;
}

void uart_puts(char *s) {
	while (*s)
        uart_putc(*s++);
}

int uart_getc() {
    if (UART_REG_READ(UART_LSR) & USR_LSR_RX_READY)
        return *UART_RBR;

    return -1;
}

char *uart_gets(char *s)
{
    char c;
    char *p = s;

    while ((c = uart_getc()) && c != '\n' && c != '\r' && c != EOF) {
        if (c == -1)
            continue;
        *p = c;
        p++;
    }

    *p = '\0';

    return s;
}

