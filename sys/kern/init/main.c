// SPDX-License-Identifier: LSL-1.4
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include "main.h"

#include "arch/amd64/cpuid.h"
#include "arch/amd64/idt.h"
#include "drivers/driver.h"
#include "fs/ext2/ext2.h"
#include "kern/framebuf/framebuf.h"
#include "kern/mb/mb.h"
#include "kern/memory/memory.h"
#include "shell/shell.h"

#include <debug/debug.h>
#include <lib/kstdio/kprint/kprint.h>

void
    start_kernel (void *mb_info) {
	idt_init();
	serial.init();

	if (mb_info == KNULL)
		__asm__("cli; hlt");

	parse_multiboot_info(mb_info);
	video.init(&fb_info);

	kmemory_init(mb_info);
	init_memory_pools();

	ext2.set_block_device(ata.pio_read, KNULL);
	if (ext2.mount(0) != 0)
		debug.err("EXT2 mount failed", "fs", KNULL);

	kcpu_init_brand();
	keyboard.init();

	__asm__ volatile("sti");

	kshell();
}
