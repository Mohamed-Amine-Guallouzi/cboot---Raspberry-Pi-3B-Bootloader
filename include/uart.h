/******************************************************************************
 * @file    uart.h
 * @author  Mohammed Amine
 * @brief   UART (Serial) driver for Raspberry Pi 3B
 *
 * Provides functions for serial communication through UART.
 * Uses PL011 UART at 0x3F201000 (Raspberry Pi 3B).
 ******************************************************************************/

#ifndef UART_H
#define UART_H

#include "types.h"

/* ============================================================================
 * UART Memory-mapped Registers (PL011) for Raspberry Pi 3B
 * ============================================================================ */

/* UART0 base address for Raspberry Pi 3B */
#define UART0_BASE 0x3F201000UL

/* Register offsets from UART0_BASE */
#define UART0_DR     (*(volatile uint32_t *)(UART0_BASE + 0x00))
#define UART0_FR     (*(volatile uint32_t *)(UART0_BASE + 0x18))
#define UART0_IBRD   (*(volatile uint32_t *)(UART0_BASE + 0x24))
#define UART0_FBRD   (*(volatile uint32_t *)(UART0_BASE + 0x28))
#define UART0_LCRH   (*(volatile uint32_t *)(UART0_BASE + 0x2C))
#define UART0_CR     (*(volatile uint32_t *)(UART0_BASE + 0x30))
#define UART0_IMSC   (*(volatile uint32_t *)(UART0_BASE + 0x38))
#define UART0_ICR    (*(volatile uint32_t *)(UART0_BASE + 0x44))

/* Flag register bits */
#define UART_FR_TXFF (1 << 5)  /* Transmit FIFO full */
#define UART_FR_RXFE (1 << 4)  /* Receive FIFO empty */

/* Line control register bits */
#define UART_LCRH_FEN (1 << 4)  /* Enable FIFOs */
#define UART_LCRH_WLEN_8BIT (3 << 5)  /* 8 data bits */

/* Control register bits */
#define UART_CR_UARTEN (1 << 0)  /* UART enable */
#define UART_CR_TXE    (1 << 8)  /* Transmit enable */
#define UART_CR_RXE    (1 << 9)  /* Receive enable */

/* Baud rate calculation for 115200 baud */
/* UART clock is 48MHz on RPi3 */
#define UART_CLOCK 48000000
#define BAUD_RATE 115200
#define BAUD_DIVISOR (UART_CLOCK / (16 * BAUD_RATE))

/* ============================================================================
 * Function Declarations
 * ============================================================================ */

void uart_init(void);
void uart_putc(char c);
void uart_puts(const char *str);
char uart_getc(void);
int uart_getc_available(void);
void uart_puthex(uint64_t value, int digits);
void uart_putdec(uint64_t value);
void uart_putbin(uint32_t value, int bits);

#endif /* UART_H */