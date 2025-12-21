###############################################################################
# Makefile for cboot (Raspberry Pi 3B Bare-metal Bootloader)
# Author: Mohammed Amine
# 
# QEMU 8.2.2 supports up to Raspberry Pi 3B
###############################################################################

# ============================================================================
# CONFIGURATION
# ============================================================================

# Build output directory
BUILD_DIR = build

# Create build directory if it doesn't exist
$(shell mkdir -p $(BUILD_DIR))

# Cross-compilation toolchain for ARM64 (bare-metal)
CC = aarch64-none-elf-gcc
LD = aarch64-none-elf-ld
OBJCOPY = aarch64-none-elf-objcopy
OBJDUMP = aarch64-none-elf-objdump

# Source files
SRC_DIR = src
SRC_FILES = $(SRC_DIR)/start.S $(SRC_DIR)/boot.c $(SRC_DIR)/uart.c

# Object files
OBJ = $(patsubst $(SRC_DIR)/%.S,$(BUILD_DIR)/%.o,$(filter %.S,$(SRC_FILES)))
OBJ += $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(filter %.c,$(SRC_FILES)))

# Include directory
INC_DIR = include

# ============================================================================
# COMPILATION FLAGS
# ============================================================================

# C Compiler flags
CFLAGS = -Wall -Wextra -O0 -ffreestanding -nostdlib -nostartfiles -I$(INC_DIR)

# Assembler flags
ASFLAGS = -ffreestanding -I$(INC_DIR)

# Linker flags
LDFLAGS = -T linker.ld -nostdlib -Map=$(BUILD_DIR)/cboot.map

# QEMU configuration for Raspberry Pi 3B
QEMU = qemu-system-aarch64
QEMU_MACHINE = raspi3b
QEMU_CPU = cortex-a53
QEMU_MEM = 1024M
QEMU_FLAGS = -M $(QEMU_MACHINE) -cpu $(QEMU_CPU) -m $(QEMU_MEM) -serial stdio -display none

# ============================================================================
# BUILD TARGETS
# ============================================================================

all: build/cboot.bin

$(BUILD_DIR)/cboot.elf: $(OBJ)
	@echo "[LD] Linking $@"
	$(LD) $(LDFLAGS) -o $@ $(OBJ)
	@echo "[INFO] ELF file created: $@"

$(BUILD_DIR)/cboot.bin: $(BUILD_DIR)/cboot.elf
	@echo "[OBJCOPY] Creating binary $@"
	$(OBJCOPY) -O binary $< $@
	@echo "[INFO] Binary file created: $@"

# ============================================================================
# PATTERN RULES
# ============================================================================

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "[CC] Compiling $<"
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.S
	@echo "[AS] Assembling $<"
	$(CC) $(ASFLAGS) -c $< -o $@

# ============================================================================
# UTILITY TARGETS
# ============================================================================

run: $(BUILD_DIR)/cboot.bin
	@echo "[QEMU] Starting Raspberry Pi 3B emulation..."
	$(QEMU) $(QEMU_FLAGS) -kernel $(BUILD_DIR)/cboot.bin

debug: $(BUILD_DIR)/cboot.elf
	@echo "[QEMU] Starting debug server on port 1234..."
	$(QEMU) $(QEMU_FLAGS) -kernel $(BUILD_DIR)/cboot.elf -S -s &
	@echo "[GDB] Starting debugger..."
	aarch64-none-elf-gdb $(BUILD_DIR)/cboot.elf -ex "target remote localhost:1234" -ex "break _start"

size: $(BUILD_DIR)/cboot.elf
	@echo "[SIZE] Memory usage:"
	aarch64-none-elf-size $(BUILD_DIR)/cboot.elf

disasm: $(BUILD_DIR)/cboot.elf
	@echo "[OBJDUMP] Generating disassembly..."
	$(OBJDUMP) -D $(BUILD_DIR)/cboot.elf > $(BUILD_DIR)/cboot.disasm
	@echo "[INFO] Disassembly saved to $(BUILD_DIR)/cboot.disasm"

qemu-test:
	@echo "[INFO] Available QEMU machine types for Raspberry Pi:"
	$(QEMU) -machine help | grep -i raspi

clean:
	@echo "[CLEAN] Removing build files..."
	rm -rf $(BUILD_DIR)/*
	@echo "[INFO] Build directory cleaned"

help:
	@echo "cboot - Raspberry Pi 3B Bare-metal Bootloader"
	@echo ""
	@echo "Available targets:"
	@echo "  all       - Build binary (default)"
	@echo "  run       - Run in QEMU"
	@echo "  debug     - Debug with GDB"
	@echo "  size      - Show memory usage"
	@echo "  disasm    - Generate disassembly"
	@echo "  qemu-test - Check available QEMU machines"
	@echo "  clean     - Remove build artifacts"
	@echo "  help      - Show this help"

# ============================================================================
# PHONY TARGETS
# ============================================================================

.PHONY: all run debug size disasm clean help qemu-test