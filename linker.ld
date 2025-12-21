/******************************************************************************
 * @file    linker.ld
 * @author  Mohammed Amine
 * @brief   Linker script for cboot - Raspberry Pi 4
 *
 * Defines memory layout and section placement for bare-metal ARM64.
 * Memory starts at 0x8000 for Raspberry Pi bootloader.
 ******************************************************************************/

/* Entry point of the program - where execution begins */
ENTRY(_start)

/* ============================================================================
 * MEMORY LAYOUT
 * ============================================================================
 *
 * RAM: 0x8000 to 0x8000 + 16M (RPi4 loads kernel at 0x80000, but we use 0x8000)
 * Note: Raspberry Pi 3/4 typically loads kernel at 0x80000 (512KB),
 * but for simplicity and testing, we're using 0x8000 (32KB).
 */
MEMORY
{
    /* RAM: Read-Write-Execute, starting at 0x8000, 16MB in size */
    RAM (rwx) : ORIGIN = 0x8000, LENGTH = 16M
}

/* ============================================================================
 * SECTION PLACEMENT
 * ============================================================================
 *
 * Sections are placed in the order they appear below.
 * This ordering is important for bootloaders.
 */

SECTIONS
{
    /* ============================= */
    /*           TEXT SECTION        */
    /* ============================= */
    
    /* Contains executable code */
    .text : {
        /* Ensure _start is at the beginning (entry point) */
        *(.text.start)      /* Special section for _start if needed */
        *(.text*)           /* All other code sections */
        
        /* Keep these sections close to code for efficiency */
        *(.text.*)
        
        /* Align to 8 bytes for ARM64 requirements */
        . = ALIGN(8);
    } > RAM
    
    /* ============================= */
    /*         READ-ONLY DATA        */
    /* ============================= */
    
    /* Contains constants, strings, and other read-only data */
    .rodata : {
        /* All read-only data sections */
        *(.rodata*)
        
        /* Align to 8 bytes */
        . = ALIGN(8);
    } > RAM
    
    /* ============================= */
    /*         INITIALIZED DATA      */
    /* ============================= */
    
    /* Contains initialized global/static variables */
    .data : {
        /* All initialized data sections */
        *(.data*)
        
        /* Align to 8 bytes */
        . = ALIGN(8);
    } > RAM
    
    /* ============================= */
    /*         UNINITIALIZED DATA    */
    /* ============================= */
    
    /* Contains uninitialized global/static variables (zero-initialized) */
    
    /* Define symbols for BSS start and end (used in startup code) */
    __bss_start = .;
    
    .bss : {
        /* All uninitialized data sections */
        *(.bss*)
        
        /* COMMON section for uninitialized global variables */
        *(COMMON)
        
        /* Align to 8 bytes */
        . = ALIGN(8);
    } > RAM
    
    __bss_end = .;
    __bss_size = __bss_end - __bss_start;
    
    /* ============================= */
    /*            STACK              */
    /* ============================= */
    
    /* Stack grows downward from the end of RAM */
    /* Reserve 64KB for stack (adjust as needed) */
    _stack_size = 64K;
    _stack_top = ORIGIN(RAM) + LENGTH(RAM);
    _stack_bottom = _stack_top - _stack_size;
    
    /* Ensure minimum stack alignment (16 bytes for ARM64) */
    _stack_top = ALIGN(16);
    
    /* ============================= */
    /*         DEBUG SECTIONS        */
    /* ============================= */
    
    /* Debug information (not loaded into memory) */
    .debug_info     0 : { *(.debug_info) }
    .debug_abbrev   0 : { *(.debug_abbrev) }
    .debug_aranges  0 : { *(.debug_aranges) }
    .debug_line     0 : { *(.debug_line) }
    .debug_frame    0 : { *(.debug_frame) }
    .debug_str      0 : { *(.debug_str) }
    .debug_loc      0 : { *(.debug_loc) }
    
    /* ============================= */
    /*         SYMBOL TABLE          */
    /* ============================= */
    
    /* Define symbols for use in C code */
    _end = .;           /* End of loaded data */
    _heap_start = .;    /* Start of heap (if implementing malloc later) */
    _heap_end = _stack_bottom;  /* Heap ends where stack begins */
}