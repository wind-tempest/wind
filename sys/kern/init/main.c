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
#include "kern/memory/memory.h"
#include "shell/shell.h"

#include <debug/debug.h>
#include <lib/kstdio/kprint/kprint.h>

// Multiboot2 header structure.
struct multiboot_header {
	kuint32_t total_size;
	kuint32_t reserved;
} __attribute__((aligned(8)));

struct multiboot_tag {
	kuint32_t type;
	kuint32_t size;
} __attribute__((aligned(8)));

struct multiboot_tag_framebuffer {
	kuint32_t type;
	kuint32_t size;
	kuint64_t addr;
	kuint32_t pitch;
	kuint32_t width;
	kuint32_t height;
	kuint8_t  bpp;
	kuint8_t  type_fb;
	kuint8_t  red_mask_size;
	kuint8_t  red_mask_shift;
	kuint8_t  green_mask_size;
	kuint8_t  green_mask_shift;
	kuint8_t  blue_mask_size;
	kuint8_t  blue_mask_shift;
	kuint8_t  reserved[2];
} __attribute__((aligned(8)));

typedef enum {
	MULTIBOOT_TAG_TYPE_END         = 0,
	MULTIBOOT_TAG_TYPE_FRAMEBUFFER = 8
} multiboot_tag_type_t;

struct framebuffer_info fb_info = {0};

// Map physical to virtual address
static void
    map_framebuffer_address (kuint64_t phys_addr) {
	kuint64_t virt_addr = 0xFFFF800000000000ULL + phys_addr;

	debug.printf("Mapping framebuffer 0x%llx -> 0x%llx\n", phys_addr, virt_addr);

	fb_info.addr = virt_addr;
}

static void
    parse_multiboot_info (void *mb_info) {
	if (mb_info == KNULL) {
		debug.uts("mb_info is NULL!");
		return;
	}

	kuint32_t total_size = *(kuint32_t *) mb_info;
	kuint8_t *current    = (kuint8_t *) ((kuintptr_t) mb_info + 8);
	kuint8_t *end        = (kuint8_t *) ((kuintptr_t) mb_info + total_size);

	debug.uts("Parsing multiboot info...");

	while (current < end) {
		struct multiboot_tag *tag = (struct multiboot_tag *) current;

		if (tag->size == 0) {
			debug.err("Invalid tag size (0)", "multiboot", KNULL);
			return;
		}

		switch ((multiboot_tag_type_t) tag->type) {
			case MULTIBOOT_TAG_TYPE_END:
				debug.uts("End tag found. Parsing complete.");
				return;

			case MULTIBOOT_TAG_TYPE_FRAMEBUFFER: {
				debug.uts("Framebuffer tag found");

				struct multiboot_tag_framebuffer *fb_tag =
				    (struct multiboot_tag_framebuffer *) tag;

				fb_info.pitch            = fb_tag->pitch;
				fb_info.width            = fb_tag->width;
				fb_info.height           = fb_tag->height;
				fb_info.bpp              = fb_tag->bpp;
				fb_info.type             = fb_tag->type_fb;
				fb_info.red_mask_size    = fb_tag->red_mask_size;
				fb_info.red_mask_shift   = fb_tag->red_mask_shift;
				fb_info.green_mask_size  = fb_tag->green_mask_size;
				fb_info.green_mask_shift = fb_tag->green_mask_shift;
				fb_info.blue_mask_size   = fb_tag->blue_mask_size;
				fb_info.blue_mask_shift  = fb_tag->blue_mask_shift;

				map_framebuffer_address(fb_tag->addr);
				break;
			}

			default:
				// Unhandled tag
				// ! DO NOT FUCKING PRINT A ERROR MESSAGE
				// ! THIS IS NOT A ERROR, JUST A UNHANDLED TAG
				break;
		}

		current += (tag->size + 7) & (kuint32_t) ~7;  // align to 8 bytes
	}

	debug.uts("Reached end of multiboot info (no END tag)");
}

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
	keyboard_init();

	__asm__ volatile("sti");

	kshell();
}
