/******************************************************************************
 * @file    uart.c
 * @author  Mohammed Amine
 * @brief   UART driver implementation for Raspberry Pi 3B
 *
 * Implements serial communication using PL011 UART.
 ******************************************************************************/

#include "uart.h"

void uart_init(void) {
    /* Disable UART first */
    UART0_CR = 0;
    
    /* Wait a bit */
    for (volatile int i = 0; i < 150; i++);
    
    /* Set baud rate to 115200 */
    /* 48MHz / (16 * 115200) = 26.041666... */
    UART0_IBRD = 26;          /* Integer part = 26 */
    UART0_FBRD = 3;           /* Fractional part = 3 */
    
    /* Enable FIFOs, 8 data bits */
    UART0_LCRH = UART_LCRH_WLEN_8BIT | UART_LCRH_FEN;
    
    /* Mask all interrupts */
    UART0_IMSC = 0;
    
    /* Enable UART, TX, and RX */
    UART0_CR = UART_CR_UARTEN | UART_CR_TXE | UART_CR_RXE;
}

void uart_putc(char c) {
    /* Wait until transmit FIFO has space */
    while (UART0_FR & UART_FR_TXFF);
    
    /* Write character */
    UART0_DR = c;
    
    /* If newline, also send carriage return */
    if (c == '\n') {
        uart_putc('\r');
    }
}

void uart_puts(const char *str) {
    while (*str) {
        uart_putc(*str++);
    }
}

char uart_getc(void) {
    /* Wait until data is available */
    while (UART0_FR & UART_FR_RXFE);
    
    /* Read and return character */
    return UART0_DR & 0xFF;
}

int uart_getc_available(void) {
    /* Return 1 if data is available, 0 otherwise */
    return !(UART0_FR & UART_FR_RXFE);
}

void uart_puthex(uint64_t value, int digits) {
    const char hex[] = "0123456789ABCDEF";
    
    for (int i = (digits - 1) * 4; i >= 0; i -= 4) {
        uart_putc(hex[(value >> i) & 0xF]);
    }
}

void uart_putdec(uint64_t value) {
    char buffer[20];
    int i = 0;
    
    if (value == 0) {
        uart_putc('0');
        return;
    }
    
    while (value > 0) {
        buffer[i++] = '0' + (value % 10);
        value /= 10;
    }
    
    while (--i >= 0) {
        uart_putc(buffer[i]);
    }
}

void uart_putbin(uint32_t value, int bits) {
    for (int i = bits - 1; i >= 0; i--) {
        uart_putc((value >> i) & 1 ? '1' : '0');
        if (i % 4 == 0 && i != 0) uart_putc(' ');
    }
}