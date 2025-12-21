# cboot - Raspberry Pi 3B Bare-metal Bootloader

A minimal bare-metal bootloader for Raspberry Pi 3B written in C and ARM64 assembly. This educational project demonstrates low-level system initialization without an operating system.

## Overview

`cboot` is a learning-oriented bootloader that shows how to:

* Initialize the ARM64 CPU and drop to EL1
* Set up stack and memory sections
* Implement a UART driver for serial communication
* Run basic memory operations in bare-metal environment
* Create an interactive debugging interface

## Features

* ARM64 startup assembly (`start.S`) - CPU initialization and EL drop
* UART driver - PL011 UART communication for Raspberry Pi 3B
* Memory utilities - memset, memcpy, strlen implementations
* Debugging tools - CPU register dump, memory layout display
* System control - Halt and reboot functionality
* Interactive console - UART echo test with commands:

  * `d` → Dump CPU registers
  * `r` → Reboot the system
  * `e` → Toggle echo mode
  * `?` → Show help

## Project Structure

```
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
```

## Build Requirements

* aarch64-none-elf-gcc
* aarch64-none-elf-ld
* aarch64-none-elf-objcopy
* qemu-system-aarch64

Debian/Ubuntu example:

```bash
sudo apt install build-essential make cmake gcc-aarch64-none-elf \
binutils-aarch64-none-elf gdb-multiarch qemu-system-arm qemu-utils git vim screen
```

## Building and Running

1. Build the bootloader:

```bash
make clean
make
```

Output binary: `build/cboot.bin`

2. Run in QEMU (Raspberry Pi 3B emulation):

```bash
make run
```

3. Debug with GDB:

```bash
make debug
```

## Bootloader Flow

```
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
```

## Usage

* UART console displays boot messages.
* Type characters to interact.
* Commands: `d`, `r`, `e`, `?`.

## Learning Outcomes

* Understand ARM64 CPU boot sequence on Raspberry Pi
* Write bare-metal startup assembly and C code
* Initialize memory and hardware without an OS
* Use UART for debugging and communication
* Learn memory sections: `.text`, `.data`, `.bss`, `.stack`

## License

Open-source. Free to use, study, and modify.
