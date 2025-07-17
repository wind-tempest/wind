CC      := ccache gcc
NASM    := nasm
LD      := ld
RM      := rm -rf

# Paths
SRC_DIR    := src
BUILD_DIR  := build
OBJDIR     := $(BUILD_DIR)/obj
OUTDIR     := $(BUILD_DIR)/out
ISODIR     := $(BUILD_DIR)/isodir

# Limine
LIMINE_BIN_DIR := boot/limine
LIMINE_FILES   := \
	$(LIMINE_BIN_DIR)/limine-bios.sys \
	$(LIMINE_BIN_DIR)/limine-bios-cd.bin \
	$(LIMINE_BIN_DIR)/limine-uefi-cd.bin \
	$(LIMINE_BIN_DIR)/BOOTX64.EFI
LIMINE_CFG := $(LIMINE_BIN_DIR)/limine.conf

ISO_PATH     := $(OUTDIR)/wind.iso
DISK_PATH    := $(BUILD_DIR)/disk.img

QEMU_COMMAND := qemu-system-x86_64 -cdrom $(ISO_PATH) \
                -vga vmware -machine hpet=on -m 512M -serial mon:stdio -drive file=$(DISK_PATH),format=raw,if=ide

# Build mode
MODE ?= Release

# Common flags
COMMON_CFLAGS   := -I$(SRC_DIR) -fno-pie -fno-stack-protector -ffreestanding -m64 \
                   -Wall -Wextra -Wpedantic -Wconversion -Werror \
                   -ffunction-sections -fdata-sections -Wundef -Wshadow
COMMON_LDFLAGS  := -no-pie -fno-stack-protector -ffreestanding -m64 -nostdinc -nostdlib \
                   -Wl,--gc-sections

# Auto-include all subdirs of src/io
IO_DIRS := $(shell find $(SRC_DIR)/io -type d)
COMMON_CFLAGS += $(patsubst %, -I%, $(IO_DIRS))

ifeq ($(MODE),Debug)
    CFLAGS  := $(COMMON_CFLAGS) -Og -g1
    LDFLAGS := $(COMMON_LDFLAGS)
else ifeq ($(MODE),Release)
    CFLAGS  := $(COMMON_CFLAGS) -O3 -march=x86-64 -mtune=generic -fno-omit-frame-pointer -flto
    LDFLAGS := $(COMMON_LDFLAGS)
else
    $(error "INVALID MODE": use Debug or Release)
endif

# Dependency flags
DEPFLAGS := -MMD -MP

# Sources
C_SRCS := $(shell find $(SRC_DIR) -name '*.c')
ASM_SRCS := \
	src/boot.asm \
	src/core/isr.asm \
	src/core/irq.asm

# Objects
C_OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJDIR)/%.o,$(C_SRCS))
ASM_OBJS := $(patsubst src/%.asm,$(OBJDIR)/%.o,$(ASM_SRCS))
OBJS := $(C_OBJS) $(ASM_OBJS)

# Super-targets
.PHONY: all clean run

all: $(ISO_PATH) $(DISK_PATH)

$(DISK_PATH):
	dd if=/dev/zero of=$(DISK_PATH) bs=1M count=64

# Add support for parallel builds
MAKEFLAGS += -j$(shell nproc)

# Build dirs
$(OBJDIR) $(OUTDIR) $(ISODIR)/boot:
	mkdir -p $@

# Compile C with deps
$(OBJDIR)/%.o: $(SRC_DIR)/%.c | $(OBJDIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

# Assemble ASM
$(OBJDIR)/%.o: src/%.asm | $(OBJDIR)
	@mkdir -p $(dir $@)
	$(NASM) -f elf64 $< -o $@

# Link ELF
$(OUTDIR)/wind.elf: $(OBJS) linker.ld | $(OUTDIR)
	$(CC) $(LDFLAGS) -T linker.ld $(OBJS) -o $@

# Create ISO
$(ISO_PATH): $(OUTDIR)/wind.elf | $(ISODIR)/boot
	cp $< $(ISODIR)/boot/wind.elf
	cp $(LIMINE_CFG) $(ISODIR)/
	cp $(LIMINE_FILES) $(ISODIR)/
	xorriso -as mkisofs \
		-o $@ \
		-b limine-bios-cd.bin \
		-no-emul-boot \
		-boot-load-size 4 \
		-boot-info-table \
		-isohybrid-mbr $(ISODIR)/limine-bios.sys \
		-eltorito-alt-boot \
		-e limine-uefi-cd.bin \
		-no-emul-boot \
		-isohybrid-gpt-basdat \
		$(ISODIR)

# Run in QEMU
run: all
	@$(QEMU_COMMAND)

# Include auto-generated deps
-include $(C_OBJS:.o=.d)

clean:
	$(RM) $(BUILD_DIR) $(DISK_PATH)
