cboot - Raspberry Pi 3B Bare-metal Bootloader
Author: Mohammed Amine
Language: C & ARM64 Assembly

========================================
Overview
========================================
cboot is a minimal bare-metal bootloader for Raspberry Pi 3B.
It demonstrates how to initialize the CPU, stack, memory, and UART
without an operating system. It also includes simple memory tests
and an interactive UART echo test for debugging.

========================================
Features
========================================
- ARM64 startup code (start.S)
- UART driver for serial communication
- Memory utilities: memset, memcpy, strlen
- CPU register dump for debugging
- System control: halt and reboot
- Interactive UART echo test with commands:
    d → Dump registers
    r → Reboot
    e → Toggle echo
    ? → Help

========================================
Project Structure
========================================
build/               # Compiled binaries and object files
include/
    boot.h           # Bootloader function declarations
    uart.h           # UART function declarations
    types.h          # Standard type definitions
linker.ld            # Linker script (memory layout, stack, BSS)
Makefile             # Build instructions
src/
    boot.c           # Main bootloader logic
    uart.c           # UART driver implementation
    start.S          # ARM64 startup assembly code

========================================
Build Requirements
========================================
- aarch64-none-elf-gcc
- aarch64-none-elf-ld
- aarch64-none-elf-objcopy
- qemu-system-aarch64

Debian/Ubuntu example:
sudo apt install build-essential make cmake gcc-aarch64-none-elf \
binutils-aarch64-none-elf gdb-multiarch qemu-system-arm qemu-utils git vim screen

========================================
Building and Running
========================================
1. Build the bootloader:
    make clean
    make
   Output binary: build/cboot.bin

2. Run in QEMU (Raspberry Pi 3B emulation):
    make run

3. Debug with GDB:
    make debug

========================================
Memory Layout
========================================
- Text: Executable code
- ROData: Constants and strings
- Data: Initialized global variables
- BSS: Uninitialized globals (zeroed at startup)
- Stack: 64 KB, grows downward

========================================
Bootloader Flow
========================================
Power On / Reset
       │
       ▼
  Raspberry Pi Firmware
  (loads kernel8.img → cboot.bin)
       │
       ▼
   ┌────────────┐
   │   _start   │  <- start.S (assembly)
   │  Setup SP  │
   │ Clear BSS  │
   │  Drop EL  │
   └────────────┘
       │
       ▼
   boot_main()   <- boot.c (C code)
       │
       │  Initialize UART
       │  Print boot messages
       │  Show exception level
       │  Display memory layout
       │
       ▼
 ┌─────────────────────────┐
 │       Memory Tests       │
 │ (memset, memcpy, strlen) │
 └─────────────────────────┘
       │
       ▼
 ┌─────────────────────────┐
 │      UART Echo Test      │
 │ Commands:                │
 │ d → Dump registers       │
 │ r → Reboot               │
 │ e → Toggle echo          │
 │ ? → Help                 │
 └─────────────────────────┘
       │
       ▼
System Halt / Reboot
       │
       ▼
   End of Bootloader

========================================
Usage
========================================
- Once booted, UART console shows boot messages.
- Type characters to interact.
- Commands: d, r, e, ?

========================================
Learning Outcomes
========================================
- How ARM64 CPU boots on Raspberry Pi
- Writing bare-metal startup code
- Initializing hardware without OS
- Understanding memory sections (.text, .data, .bss, .stack)
- Using UART for debugging and interaction

========================================
License
========================================
Open-source. Free to use, study, and modify.
