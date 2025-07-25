# Compiler tools
CC      := ccache clang
NASM    := nasm
LD      := ld
RM      := rm -rf

# Directory structure
SRC_DIR     := src
BUILD_DIR   := build
OBJDIR      := $(BUILD_DIR)/obj
OUTDIR      := $(BUILD_DIR)/out
ISODIR      := $(BUILD_DIR)/isodir
BOOTDIR     := $(ISODIR)/boot

# Limine
LIMINE_DIR   := boot/limine
LIMINE_FILES := $(LIMINE_DIR)/limine-bios.sys \
                $(LIMINE_DIR)/limine-bios-cd.bin \
                $(LIMINE_DIR)/limine-uefi-cd.bin \
                $(LIMINE_DIR)/BOOTX64.EFI
LIMINE_CFG   := $(LIMINE_DIR)/limine.conf

# Outputs
ISO_PATH   := $(OUTDIR)/wind.iso
DISK_PATH  := $(BUILD_DIR)/disk.img

# QEMU
QEMU_COMMAND := qemu-system-x86_64 -cdrom $(ISO_PATH) \
                -vga none -machine hpet=on -m 512M \
                -serial mon:stdio -drive file=$(DISK_PATH),format=raw,if=ide

# Build mode
MODE ?= Release

# C Flags
COMMON_CFLAGS := -I$(SRC_DIR) -fno-pie -fno-stack-protector -ffreestanding \
                 -m64 -std=gnu23 -Wall -Wextra -Wpedantic -Wconversion -Werror \
                 -ffunction-sections -fdata-sections -Wundef -Wshadow \
                 -Wno-unused-command-line-argument

IO_DIRS := $(shell find $(SRC_DIR)/io -type d)
COMMON_CFLAGS += $(patsubst %, -I%, $(IO_DIRS))

ifeq ($(MODE),Debug)
    CFLAGS  := $(COMMON_CFLAGS) -Og -g1
else ifeq ($(MODE),Release)
    CFLAGS  := $(COMMON_CFLAGS) -O3 -march=x86-64 -mtune=generic -fno-omit-frame-pointer
else
    $(error "INVALID MODE": use Debug or Release)
endif

LDFLAGS := -no-pie -fno-stack-protector -ffreestanding -m64 -nostdinc -nostdlib -Wl,--gc-sections

# Dependency generation
DEPFLAGS := -MMD -MP

# Sources
C_SRCS   := $(shell find $(SRC_DIR) -name '*.c')
ASM_SRCS := $(shell find $(SRC_DIR) -name '*.asm')

C_OBJS   := $(patsubst $(SRC_DIR)/%.c,$(OBJDIR)/%.o,$(C_SRCS))
ASM_OBJS := $(patsubst $(SRC_DIR)/%.asm,$(OBJDIR)/%.o,$(ASM_SRCS))
OBJS     := $(C_OBJS) $(ASM_OBJS)

# Build everything by default
.PHONY: all clean run
all: $(ISO_PATH) $(DISK_PATH)

# Create required directories
$(OBJDIR) $(OUTDIR) $(BOOTDIR):
	mkdir -p $@

# Compile C files
$(OBJDIR)/%.o: $(SRC_DIR)/%.c | $(OBJDIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

# Assemble ASM files
$(OBJDIR)/%.o: $(SRC_DIR)/%.asm | $(OBJDIR)
	@mkdir -p $(dir $@)
	$(NASM) -f elf64 $< -o $@

# Link
$(OUTDIR)/wind.elf: $(OBJS) src/linker.ld | $(OUTDIR)
	$(CC) $(LDFLAGS) -T src/linker.ld $(OBJS) -o $@
ifeq ($(MODE),Release)
	strip --strip-debug $@
endif

# Generate ISO
$(ISO_PATH): $(OUTDIR)/wind.elf | $(BOOTDIR)
	cp $< $(BOOTDIR)/wind.elf
	cp $(LIMINE_CFG) $(ISODIR)/
	cp $(LIMINE_FILES) $(ISODIR)/
	xorriso -as mkisofs \
		-o $@ \
		-b limine-bios-cd.bin -no-emul-boot \
		-boot-load-size 4 -boot-info-table \
		-isohybrid-mbr $(ISODIR)/limine-bios.sys \
		-eltorito-alt-boot \
		-e limine-uefi-cd.bin -no-emul-boot \
		-isohybrid-gpt-basdat \
		$(ISODIR)

# Create dummy disk image
$(DISK_PATH):
	dd if=/dev/zero of=$@ bs=1M count=64

# Run with QEMU
run: all
	@$(QEMU_COMMAND)

# Include dependencies
-include $(C_OBJS:.o=.d)

# Clean
clean:
	$(RM) $(BUILD_DIR) $(DISK_PATH)
