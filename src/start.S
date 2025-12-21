/******************************************************************************
 * @file    start.S
 * @author  Mohammed Amine
 * @brief   ARM64 assembly startup code for Raspberry Pi 4
 *
 * This is the entry point of the bootloader. It sets up the basic
 * environment and jumps to the C main function.
 ******************************************************************************/

/* ============================================================================
 * DEFINES
 * ============================================================================ */

/* Current Exception Level (EL) */
#define CURRENT_EL_EL1    (1 << 2)
#define CURRENT_EL_EL2    (2 << 2)
#define CURRENT_EL_EL3    (3 << 2)

/* ============================================================================
 * SECTION DECLARATIONS
 * ============================================================================ */

/* Define a special section for startup code to ensure it's placed first */
.section .text.start, "ax"
.global _start

/* ============================================================================
 * ENTRY POINT
 * ============================================================================ */

/**
 * _start - Bootloader entry point
 *
 * This is where the Raspberry Pi firmware jumps after initial boot.
 * The CPU starts here in EL2 (Hypervisor mode) on RPi4.
 */
_start:
    /* ------------------------------------------------------------------------
     * Step 1: Check current exception level and drop to EL1 if needed
     * ------------------------------------------------------------------------ */
    
    /* Read Current Exception Level (EL) from CurrentEL register */
    mrs x0, CurrentEL
    
    /* Check if we're in EL2 */
    cmp x0, #CURRENT_EL_EL2
    b.eq drop_to_el1
    
    /* Check if we're in EL3 */
    cmp x0, #CURRENT_EL_EL3
    b.eq drop_to_el2
    
    /* If we're already in EL1, continue */
    b continue_startup
    
drop_to_el3:
    /* TODO: Implement EL3 to EL2 drop if needed */
    b system_halt
    
drop_to_el2:
    /* TODO: Implement EL2 to EL1 drop if needed */
    b continue_startup
    
drop_to_el1:
    /* Configure necessary system registers for EL1 */
    mov x0, #(1 << 31)          /* Set AArch64 execution state for EL1 */
    msr hcr_el2, x0             /* Write to Hypervisor Configuration Register */
    
    mov x0, #0x3C5              /* DAIF=1111 (disable interrupts), EL1h mode */
    msr spsr_el2, x0            /* Saved Program Status Register for EL2 */
    
    adr x0, continue_startup    /* Address to return to after exception */
    msr elr_el2, x0             /* Exception Link Register for EL2 */
    
    eret                        /* Return to EL1 */

continue_startup:
    /* ------------------------------------------------------------------------
     * Step 2: Setup stack pointer
     * ------------------------------------------------------------------------ */
    
    /* Load stack top address from linker script symbol */
    ldr x0, =_stack_top
    mov sp, x0                  /* Set stack pointer */
    
    /* ------------------------------------------------------------------------
     * Step 3: Clear BSS section (zero uninitialized data)
     * ------------------------------------------------------------------------ */
    
    ldr x0, =__bss_start        /* Start of BSS */
    ldr x1, =__bss_end          /* End of BSS */
    mov x2, #0                  /* Fill value (zero) */
    
clear_bss_loop:
    cmp x0, x1                  /* Compare current address with end */
    b.ge clear_bss_done         /* If x0 >= x1, we're done */
    
    str x2, [x0], #8            /* Store zero and increment by 8 bytes */
    b clear_bss_loop            /* Repeat */

clear_bss_done:
    /* ------------------------------------------------------------------------
     * Step 4: Call early initialization (if any)
     * ------------------------------------------------------------------------ */
    
    /* Any architecture-specific early init can go here */
    
    /* ------------------------------------------------------------------------
     * Step 5: Jump to C main function
     * ------------------------------------------------------------------------ */
    
    bl boot_main                /* Call C entry point */
    
    /* ------------------------------------------------------------------------
     * Step 6: Halt if boot_main returns (shouldn't happen)
     * ------------------------------------------------------------------------ */
    
    b system_halt               /* Infinite loop if main returns */

/* ============================================================================
 * SYSTEM HALT FUNCTION
 * ============================================================================ */

/**
 * system_halt - Stop CPU execution
 *
 * This function puts the CPU in an infinite loop.
 * Called on fatal errors or when bootloader completes its task.
 */
system_halt:
    wfi                         /* Wait for interrupt (low-power) */
    b system_halt               /* Loop forever */
    
    /* This function should never return */
    ret

/* ============================================================================
 * DATA SECTION (if needed)
 * ============================================================================ */

/* Example of placing data in .data section */
.section .data
.align 8
boot_message:
    .asciz "cboot - Raspberry Pi 4 Bootloader\r\n"