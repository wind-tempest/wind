// kernel.c

/* ============================================================================
 * Wind Operating System / Tempest Kernel
 * Copyright (C) 2025 Wind/Tempest Foundation <https://wind.infernointeractive.win>
 * ============================================================================
 * 
 * - Overview:
 *   This source file is part of the Wind (Operating System) and Tempest (Kernel), a fully
 *   open-source operating system kernel developed to provide a robust, scalable,
 *   and efficient platform optimized for modern hardware and user-centric freedom.
 * 
 * - Project Scope:
 *   Wind/Tempest targets a modular architecture supporting symmetric multiprocessing,
 *   preemptive multitasking, fine-grained memory management, and extensible driver
 *   frameworks. The system emphasizes low latency, high throughput, and
 *   predictable behavior for both user-space applications and kernel subsystems.
 * 
 * - Licensing Terms:
 * The Wind/Tempest source code is licensed under the Liberty Software License (LSL) version 1.0,
 *   a free software license based on the GNU Affero General Public License v3 (AGPLv3)
 *   with added provisions to ensure transparency, prevent DRM, and prohibit
 *   unauthorized AI training uses.
 * 
 *   You are authorized to copy, modify, distribute, and use this software,
 *   provided that all modifications and derivatives comply with the LSL terms.
 *   Inclusion of proper attribution and licensing notices is mandatory in redistributions.
 * 
 * - Warranty and Liability Disclaimer:
 *   This software is provided "as-is", without any express or implied warranty,
 *   including but not limited to implied warranties of merchantability, fitness for
 *   a particular purpose, or non-infringement of third-party rights.
 *   Under no circumstances shall the authors, contributors, or copyright holders be liable
 *   for any damages arising directly or indirectly from the use or inability to use
 *   this software, including data loss, system failures, or other incidental damages.
 * 
 * - Contribution Guidelines:
 *   Contributions to the Wind/Tempest project are welcome and encouraged.
 *   Contributors must agree to license their contributions under the same LSL terms.
 *   When submitting patches, please ensure they adhere to the project's coding standards,
 *   include proper documentation, and maintain backward compatibility where possible.
 * 
 * - Security and Stability:
 *   The kernel maintains rigorous coding practices to minimize security vulnerabilities.
 *   Regular code audits, testing, and community reviews are integral to the project's
 *   ongoing stability and security posture.
 *   Users and developers should report issues promptly via official channels.
 * 
 * - Documentation and Support:
 *   Comprehensive documentation including API references, architecture overviews,
 *   and development guides are available on the official website:
 *     https://wind.infernointeractive.win
 *   Community support can be found through mailing lists, forums, and issue trackers
 *   hosted on the project's main repository.
 * 
 * - Additional Notes:
 *   This kernel is designed to be hardware agnostic, supporting a wide range of
 *   architectures and devices. Modularity in subsystems allows customization
 *   for embedded, desktop, and server deployments.
 * 
 * By using or modifying this code, you agree to uphold the principles of free and
 * open-source software, contributing back improvements to foster collective progress.
 */

#include "kernel.h"

#include "core/cpu/cpu.h"
#include "core/cpu/idt.h"
#include "core/memory/memory.h"
#include "drivers/ata/ata.h"
#include "drivers/driver.h"
#include "fs/ext2/ext2.h"
#include "kdebug.h"
#include "kprint.h"
#include "shell/shell.h"

kbool kuse_debug = kfalse;

// Multiboot2 header structure.
struct multiboot_header {
	kuint32_t total_size;
	kuint32_t reserved;
};

// Multiboot2 tag structure.
struct multiboot_tag {
	kuint32_t type;
	kuint32_t size;
};

// Multiboot2 framebuffer tag.
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
};

// Multiboot2 tag types.
#define MULTIBOOT_TAG_TYPE_END	       0
#define MULTIBOOT_TAG_TYPE_FRAMEBUFFER 8

struct framebuffer_info fb_info = {0};

// Function to map a physical address to virtual address in page tables.
static void
    map_framebuffer_address (kuint64_t phys_addr) {
	kdebugf(
	    "Mapping framebuffer address 0x%llx to virtual address 0x%llx\n",
	    phys_addr,
	    0xFFFF800000000000 + phys_addr);

	fb_info.addr = 0xFFFF800000000000ULL + phys_addr;

	kdebugf("Framebuffer mapped to virtual address 0x%llx\n", fb_info.addr);
}

static void
    parse_multiboot_info (void *mb_info) {
	if ( mb_info == KNULL ) {
		kduts("mb_info is NULL!");
		return;
	}

	kuint32_t total_size	  = *(kuint32_t *) mb_info;
	kuint8_t *current_tag_ptr = (kuint8_t *) ((kuintptr_t) mb_info + 8);
	kuint8_t *end_of_tags	  = (kuint8_t *) ((kuintptr_t) mb_info + total_size);

	kduts("Parsing multiboot info...");

	while ( current_tag_ptr < end_of_tags ) {
		struct multiboot_tag *tag = (struct multiboot_tag *) current_tag_ptr;

		kuint32_t tag_size = tag->size;
		if ( tag_size == 0 ) {
			kduts("Error: Invalid tag size 0.");
			return;
		}

		switch ( tag->type ) {
			case MULTIBOOT_TAG_TYPE_END:
				kduts(
				    "Found end tag. Multiboot parsing "
				    "complete.");
				return;

			case MULTIBOOT_TAG_TYPE_FRAMEBUFFER: {
				kduts("Found framebuffer tag!");
				struct multiboot_tag_framebuffer *fb_tag =
				    (struct multiboot_tag_framebuffer *) tag;

				fb_info.addr		 = fb_tag->addr;
				fb_info.pitch		 = fb_tag->pitch;
				fb_info.width		 = fb_tag->width;
				fb_info.height		 = fb_tag->height;
				fb_info.bpp		 = fb_tag->bpp;
				fb_info.type		 = fb_tag->type_fb;
				fb_info.red_mask_size	 = fb_tag->red_mask_size;
				fb_info.red_mask_shift	 = fb_tag->red_mask_shift;
				fb_info.green_mask_size	 = fb_tag->green_mask_size;
				fb_info.green_mask_shift = fb_tag->green_mask_shift;
				fb_info.blue_mask_size	 = fb_tag->blue_mask_size;
				fb_info.blue_mask_shift	 = fb_tag->blue_mask_shift;

				map_framebuffer_address(fb_info.addr);
				break;
			}

			default:
				/*
				 * This is not an error, just an unhandled tag.
				 * We can print info about
				 * it if we want, in case "kduts("Unhandled tag found");"
				 */
				break;
		}

		// Move to the next tag, ensuring 8-byte alignment.
		current_tag_ptr += (tag_size + 7) & 0xFFFFFFF8;
	}

	kduts(
	    "Finished parsing multiboot tags (end tag not found, but reached "
	    "end of info).");
}

void
    start_kernel (void *mb_info) {
	/* In this case, we must use kuse_debug instead of the functions that
	 * check debug.
	 * Initialize crucial parts first. The IDT must be loaded before
	 * any hardware is touched to prevent triple faults.
	 */
	idt_init();
	serial_init();

	if ( mb_info == KNULL ) {
		/*
		 * We can't do anything without multiboot info.
		 * We can't even print an error.
		 * There is nothing we can do.
		 */
		__asm__("cli; hlt");
	}

	// Parse multiboot information.
	parse_multiboot_info(mb_info);

	// Initialize framebuffer if available.
	if ( fb_info.addr != 0 ) {
		kvideo_init(&fb_info);
	}

	// Initialize memory manager
	kmemory_init(mb_info);
	init_memory_pools();

	// Initialize ATA and mount EXT2 disk
	kext2_set_block_device(ata_pio_read, KNULL);
	if ( kext2_mount(0) != 0 ) {
		kerror("EXT2 mount failed", KNULL);
	}

	// Initialize CPU brand string
	kcpu_init_brand();

	// Initialize keyboard before enabling interrupts
	keyboard_init();

	// Enable interrupts now that all basic drivers are loaded.
	__asm__ volatile("sti");

	kshell();
}
