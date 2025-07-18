CC      := ccache clang
NASM    := nasm
LD      := ld
RM      := rm -rf

SRC_DIR    := src
BUILD_DIR  := build
OBJDIR     := $(BUILD_DIR)/obj
OUTDIR     := $(BUILD_DIR)/out
ISODIR     := $(BUILD_DIR)/isodir

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

MODE ?= Release

COMMON_CFLAGS   := -I$(SRC_DIR) -fno-pie -fno-stack-protector -ffreestanding -m64 \
                   -Wall -Wextra -Wpedantic -Wconversion -Werror \
                   -ffunction-sections -fdata-sections -Wundef -Wshadow -Wno-unused-command-line-argument

COMMON_LDFLAGS  := -no-pie -fno-stack-protector -ffreestanding -m64 -nostdinc -nostdlib \
                   -Wl,--gc-sections

IO_DIRS := $(shell find $(SRC_DIR)/io -type d)
COMMON_CFLAGS += $(patsubst %, -I%, $(IO_DIRS))

ifeq ($(MODE),Debug)
    CFLAGS  := $(COMMON_CFLAGS) -Og -g1
    LDFLAGS := $(COMMON_LDFLAGS)
else ifeq ($(MODE),Release)
    CFLAGS  := $(COMMON_CFLAGS) -O3 -march=x86-64 -mtune=generic -fno-omit-frame-pointer
    LDFLAGS := $(COMMON_LDFLAGS)
else
    $(error "INVALID MODE": use Debug or Release)
endif

DEPFLAGS := -MMD -MP

C_SRCS := $(shell find $(SRC_DIR) -name '*.c')
ASM_SRCS := \
	src/boot.asm \
	src/core/isr.asm \
	src/core/irq.asm

C_OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJDIR)/%.o,$(C_SRCS))
ASM_OBJS := $(patsubst src/%.asm,$(OBJDIR)/%.o,$(ASM_SRCS))
OBJS := $(C_OBJS) $(ASM_OBJS)

.PHONY: all clean run

all: $(ISO_PATH) $(DISK_PATH)

$(DISK_PATH):
	dd if=/dev/zero of=$(DISK_PATH) bs=1M count=64

MAKEFLAGS += -j$(shell nproc)

$(OBJDIR) $(OUTDIR) $(ISODIR)/boot:
	mkdir -p $@

$(OBJDIR)/%.o: $(SRC_DIR)/%.c | $(OBJDIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

$(OBJDIR)/%.o: src/%.asm | $(OBJDIR)
	@mkdir -p $(dir $@)
	$(NASM) -f elf64 $< -o $@

$(OUTDIR)/wind.elf: $(OBJS) linker.ld | $(OUTDIR)
	$(CC) $(LDFLAGS) -T linker.ld $(OBJS) -o $@
ifeq ($(MODE),Release)
	strip --strip-debug $@
endif

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

run: all
	@$(QEMU_COMMAND)

-include $(C_OBJS:.o=.d)

clean:
	$(RM) $(BUILD_DIR) $(DISK_PATH)
