/******************************************************************************
 * @file    boot.c
 * @author  Mohammed Amine
 * @brief   Main bootloader logic for Raspberry Pi 4
 *
 * This is the C entry point of the bootloader.
 ******************************************************************************/

#include "boot.h"
#include "uart.h"
#include "types.h"

/* ============================================================================
 * Linker Symbols (defined in linker.ld)
 * ============================================================================ */

/* These are defined in the linker script */
extern uint64_t __bss_start;
extern uint64_t __bss_end;
extern uint64_t __bss_size;
extern uint64_t _stack_top;
extern uint64_t _stack_bottom;

/* ============================================================================
 * Memory Functions
 * ============================================================================ */

/**
 * @brief Simple memory set function
 * 
 * @param ptr Pointer to memory area
 * @param value Value to set
 * @param size Number of bytes to set
 */
static void my_memset(void *ptr, int value, size_t size) {
    uint8_t *p = (uint8_t *)ptr;
    for (size_t i = 0; i < size; i++) {
        p[i] = value;
    }
}

/**
 * @brief Simple memory copy function
 * 
 * @param dest Destination pointer
 * @param src Source pointer
 * @param size Number of bytes to copy
 */
static void my_memcpy(void *dest, const void *src, size_t size) {
    uint8_t *d = (uint8_t *)dest;
    const uint8_t *s = (const uint8_t *)src;
    for (size_t i = 0; i < size; i++) {
        d[i] = s[i];
    }
}

/**
 * @brief Simple string length function
 * 
 * @param str Null-terminated string
 * @return Length of string (excluding null terminator)
 */
static size_t my_strlen(const char *str) {
    size_t len = 0;
    while (str[len]) len++;
    return len;
}

/* ============================================================================
 * System Functions
 * ============================================================================ */

/**
 * @brief Get current exception level
 * 
 * @return Current exception level (0-3)
 */
uint32_t get_current_el(void) {
    uint64_t current_el;
    asm volatile("mrs %0, CurrentEL" : "=r" (current_el));
    return (current_el >> 2) & 0x3;
}

/**
 * @brief Delay function
 * 
 * @param cycles Number of delay cycles
 * 
 * Implements a busy-wait delay.
 */
void delay(uint64_t cycles) {
    while (cycles--) {
        asm volatile("nop");
    }
}

/**
 * @brief Early system initialization
 */
void early_init(void) {
    /* Nothing needed for now - UART init will handle hardware setup */
}

/**
 * @brief System halt function
 */
void system_halt(void) {
    /* Disable interrupts */
    asm volatile("msr daifset, #0xf" ::: "memory");
    
    /* Enter low-power infinite loop */
    uart_puts("\n[SYSTEM] Halting CPU...\n");
    while (1) {
        asm volatile("wfi");
    }
    
    __builtin_unreachable();
}

/**
 * @brief Dump registers for debugging
 */
void dump_registers(void) {
    uint64_t regs[31];
    
    uart_puts("\n[DEBUG] Register Dump:\n");
    uart_puts("====================\n");
    
    /* Get general-purpose registers */
    asm volatile(
        "str x0, [%0, #0]\n"
        "str x1, [%0, #8]\n"
        "str x2, [%0, #16]\n"
        "str x3, [%0, #24]\n"
        "str x4, [%0, #32]\n"
        "str x5, [%0, #40]\n"
        "str x6, [%0, #48]\n"
        "str x7, [%0, #56]\n"
        "str x8, [%0, #64]\n"
        "str x9, [%0, #72]\n"
        "str x10, [%0, #80]\n"
        "str x11, [%0, #88]\n"
        "str x12, [%0, #96]\n"
        "str x13, [%0, #104]\n"
        "str x14, [%0, #112]\n"
        "str x15, [%0, #120]\n"
        "str x16, [%0, #128]\n"
        "str x17, [%0, #136]\n"
        "str x18, [%0, #144]\n"
        "str x19, [%0, #152]\n"
        "str x20, [%0, #160]\n"
        "str x21, [%0, #168]\n"
        "str x22, [%0, #176]\n"
        "str x23, [%0, #184]\n"
        "str x24, [%0, #192]\n"
        "str x25, [%0, #200]\n"
        "str x26, [%0, #208]\n"
        "str x27, [%0, #216]\n"
        "str x28, [%0, #224]\n"
        "str x29, [%0, #232]\n"
        "str x30, [%0, #240]\n"
        : : "r" (regs) : "memory"
    );
    
    /* Display registers */
    for (int i = 0; i < 31; i++) {
        uart_puts("x");
        if (i < 10) uart_putc('0' + i);
        else uart_putc('A' + (i - 10));
        uart_puts(": 0x");
        uart_puthex(regs[i], 8);
        uart_puts("\n");
    }
}

/**
 * @brief Reboot the system
 * 
 * Triggers a system reset through the watchdog timer.
 * Note: This is a simulated reboot in QEMU.
 */
/**
 * @brief Soft reset/reboot the system
 * 
 * Performs a software reset by jumping back to the entry point.
 * This simulates a reboot in QEMU.
 */
void system_reboot(void) {
    uart_puts("\r\n[SYSTEM] Rebooting system...\r\n");
    
    /* Small delay to see the message */
    delay(1000000);
    
    uart_puts("[SYSTEM] Resetting CPU...\r\n\r\n");
    
    /* Flush UART output */
    delay(10000);
    
    /* Method 1: Jump to reset vector (simplest for QEMU) */
    /* In QEMU, we can simulate reset by jumping to _start */
    asm volatile(
        "mov x0, #0x8000\n"   /* Entry point address from linker.ld */
        "br x0\n"
    );
    
    /* Should never reach here */
    while(1);
}

/* ============================================================================
 * Main Bootloader Function
 * ============================================================================ */

void boot_main(void) {
    /* Initialize UART first so we can output messages */
    uart_init();
    
    /* Send boot message */
    uart_puts("\r\n");
    uart_puts("========================================\r\n");
    uart_puts("cboot - Raspberry Pi 3B Bootloader\r\n");  // Changed from Pi 4 to Pi 3B
    uart_puts("Author: Mohammed Amine\r\n");
    uart_puts("Build: " __DATE__ " " __TIME__ "\r\n");
    uart_puts("========================================\r\n\r\n");
    
    /* Display system information */
    uart_puts("[SYSTEM] Initializing...\r\n");
    
    /* Show exception level */
    uint32_t el = get_current_el();
    uart_puts("[SYSTEM] Exception Level: EL");
    uart_putc('0' + el);
    uart_puts("\r\n");
    
    /* Show memory layout */
    uart_puts("[MEMORY] BSS Section: ");
    uart_puts("start=0x");
    uart_puthex((uint64_t)&__bss_start, 8);
    uart_puts(", end=0x");
    uart_puthex((uint64_t)&__bss_end, 8);
    uart_puts(", size=");
    uart_putdec((uint64_t)&__bss_size);
    uart_puts(" bytes\r\n");
    
    uart_puts("[MEMORY] Stack: ");
    uart_puts("top=0x");
    uart_puthex((uint64_t)&_stack_top, 8);
    uart_puts(", bottom=0x");
    uart_puthex((uint64_t)&_stack_bottom, 8);
    uart_puts("\r\n");
    
    /* Test memory functions */
    uart_puts("\r\n[TEST] Testing memory functions...\r\n");
    
    char buffer[32];
    const char *test_str = "Hello, Raspberry Pi 3!";
    
    /* Test memset */
    my_memset(buffer, 0, sizeof(buffer));
    uart_puts("[TEST] memset: OK\r\n");
    
    /* Test memcpy and strlen */
    size_t len = my_strlen(test_str);
    my_memcpy(buffer, test_str, len + 1);
    uart_puts("[TEST] memcpy: OK\r\n");
    
    uart_puts("[TEST] String length: ");
    uart_putdec(len);
    uart_puts("\r\n");
    
    /* Display test string */
    uart_puts("[TEST] String test: ");
    uart_puts(buffer);
    uart_puts("\r\n");
    
    /* Test hex output */
    uart_puts("[TEST] Hex test: 0x");
    uart_puthex(0xDEADBEEF, 8);
    uart_puts("\r\n");
    
    /* Test decimal output */
    uart_puts("[TEST] Decimal test: ");
    uart_putdec(123456789);
    uart_puts("\r\n");
    
    /* Simple echo test */
    uart_puts("\r\n[ECHO] Type characters (ESC to exit, ? for help):\r\n");
    uart_puts("> ");
    
    char c;
    int echo_on = 1;
    
    while (1) {
        if (uart_getc_available()) {
            c = uart_getc();
            
            /* Handle special keys */
            switch (c) {
                case 0x1B:  /* ESC key */
                    uart_puts("\r\n[SYSTEM] Exiting echo test\r\n");
                    break;
                    
                case '?':   /* Help */
                    uart_puts("\r\n[HELP] Commands:\r\n");
                    uart_puts("  ESC - Exit echo test\r\n");
                    uart_puts("  ?   - This help\r\n");
                    uart_puts("  d   - Dump registers\r\n");
                    uart_puts("  r   - Reboot system\r\n");
                    uart_puts("  e   - Toggle echo (");
                    uart_puts(echo_on ? "ON" : "OFF");
                    uart_puts(")\r\n");
                    uart_puts("> ");
                    continue;
                    
                case 'd':   /* Dump registers */
                    dump_registers();
                    uart_puts("> ");
                    continue;
                    
                case 'e':   /* Toggle echo */
                    echo_on = !echo_on;
                    uart_puts("\r\n[ECHO] Echo ");
                    uart_puts(echo_on ? "ON" : "OFF");
                    uart_puts("\r\n> ");
                    continue;
                    
                    case 'r':   /* Reboot */
                    uart_puts("\r\n[SYSTEM] Rebooting system...\r\n");
                    delay(5000000);
                    /* Jump back to start */
                    asm volatile("mov x0, #0x8000\n"
                                 "br x0");
                    break;
                    
                default:
                    if (echo_on) {
                        uart_putc(c);
                    }
                    continue;
            }
            
            /* If we get here, ESC was pressed */
            break;
        }
    }
    
    /* Completion message */
    uart_puts("\r\n[SYSTEM] Bootloader completed successfully\r\n");
    uart_puts("[SYSTEM] Entering system halt...\r\n");
    
    /* Halt the system */
    system_halt();
}