
#include <stdint.h>

#define UART        0x10000000
#define UART_THR    (uint8_t *)(UART + 0x00) // THR: transmitter holding register
#define UART_RBR    (uint8_t *)(UART + 0x00) // RBR: receiver buffer register
#define UART_IER    (uint8_t *)(UART + 0x01) // IER: interrupt enable register
#define UART_FCR    (uint8_t *)(UART + 0x02) // FCR: FIFO control register
#define UART_LCR    (uint8_t *)(UART + 0x03) // LCR: line control register
#define UART_LSR    (uint8_t *)(UART + 0x05) // LSR: line status register


#define UART_LSR_EMPTY_MASK 0x40             // LSR Bit 6: Transmitter empty; both the THR and LSR are empty


#define USR_LSR_RX_READY 0x01                // USR Bit 0: Receiver ready

#define EOF 0

void uart_init();
void uart_puts(char *s);