/******************************************************************************
 * @file    boot.h
 * @author  Mohammed Amine
 * @brief   Bootloader definitions for Raspberry Pi 3B
 *
 * Contains function declarations and constants for the bootloader.
 ******************************************************************************/

#ifndef BOOT_H
#define BOOT_H

#include "types.h"

/* ============================================================================
 * System Constants
 * ============================================================================ */

/* Raspberry Pi 3B peripheral base address */
#define PERIPHERAL_BASE 0x3F000000UL

/* Linker symbols */
extern uint64_t __bss_start;
extern uint64_t __bss_end;
extern uint64_t __bss_size;
extern uint64_t _stack_top;
extern uint64_t _stack_bottom;

/* ============================================================================
 * Function Declarations
 * ============================================================================ */

void boot_main(void);
void early_init(void);
void system_halt(void) __attribute__((noreturn));
void delay(uint64_t cycles);
uint32_t get_current_el(void);
void dump_registers(void);

#endif /* BOOT_H */