# cboot — Raspberry Pi 3B Bare-metal Bootloader

Author: Mohammed Amine
Language: C & ARM64 Assembly

Overview

cboot is a minimal bare-metal bootloader for the Raspberry Pi 3B, written entirely in C and ARM64 assembly.

It demonstrates how to:

Initialize the CPU and stack without an operating system.

Set up memory and clear the BSS section.

Initialize UART for serial output.

Interact with the system through a simple echo test.

Implement basic low-level system functions like reboot and halt.

This project is designed for learning purposes and provides a solid foundation for embedded systems programming on ARM64.

Features

ARM64 startup code (start.S) for Raspberry Pi 3B

UART driver for serial communication

Memory utilities: memset, memcpy, strlen

CPU register dump for debugging

System control functions: halt and soft reboot

Interactive echo test with simple commands:

d → Dump CPU registers

r → Reboot system

e → Toggle echo

? → Display help

Project Structure
cboot/
├── build/               # Compiled binaries and object files
├── include/
│   ├── boot.h           # Bootloader function declarations
│   ├── uart.h           # UART function declarations
│   └── types.h          # Standard type definitions
├── linker.ld            # Linker script (memory layout, stack, BSS)
├── Makefile             # Build instructions
├── README.md
└── src/
    ├── boot.c           # Main bootloader logic
    ├── uart.c           # UART driver implementation
    └── start.S          # ARM64 startup assembly code

Build Requirements

You need a cross-compilation toolchain for ARM64 (bare-metal):

aarch64-none-elf-gcc

aarch64-none-elf-ld

aarch64-none-elf-objcopy

qemu-system-aarch64 (for emulation)

On Debian/Ubuntu:

sudo apt install build-essential make cmake gcc-aarch64-none-elf \
binutils-aarch64-none-elf gdb-multiarch qemu-system-arm qemu-utils git vim screen

Building and Running

Build the bootloader

make clean
make


The output binary will be in build/cboot.bin.

Run in QEMU (Raspberry Pi 3B emulation)

make run


or directly:

qemu-system-aarch64 -M raspi3b -cpu cortex-a53 -m 1024M -serial stdio -display none -kernel build/cboot.bin


Debugging with GDB

make debug


Starts QEMU paused and connects GDB for step-by-step debugging.

Memory Layout

Text: Executable code

ROData: Constants and strings

Data: Initialized global variables

BSS: Uninitialized globals (zeroed at startup)

Stack: 64 KB, grows downward

Stack and BSS addresses are defined in linker.ld.

Usage

Once booted in QEMU, the UART console shows boot messages.

Type characters to see echo test.

Use commands:

d → Dump registers

r → Reboot bootloader

e → Toggle echo

? → Help

Learning Outcomes

By studying and running cboot, you will learn:

How an ARM64 CPU boots on Raspberry Pi.

How to write bare-metal startup code.

How to initialize hardware without OS.

How memory sections (.text, .data, .bss, .stack) work in embedded systems.

How to use UART for debugging and interaction.

License

This project is open-source and free to use. Feel free to modify, study, and experiment.
