/* kernel.c */

/*
 * Copyright (C) 2025 Wind/Tempest Foundation
 *
 * This file is part of Wind/Tempest.
 *
 * Wind/Tempest is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Wind/Tempest is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "cpu.h"
#include "dlog.h"
#include "drivers/ata/ata.h"
#include "drivers/driver.h"
#include "fs/ext2/ext2.h"
#include "idt.h"
#include "kprint.h"
#include "memory/memory.h"
#include "shell/shell.h"

/* FIXED! FINALLY! Just don't use debug before the video initialization. */
bool use_debug = false;

/* Multiboot2 header structure. */
struct multiboot_header
{
	uint32_t total_size;
	uint32_t reserved;
};

/* Multiboot2 tag structure. */
struct multiboot_tag
{
	uint32_t type;
	uint32_t size;
};

/* Multiboot2 framebuffer tag. */
struct multiboot_tag_framebuffer
{
	uint32_t type;
	uint32_t size;
	uint64_t addr;
	uint32_t pitch;
	uint32_t width;
	uint32_t height;
	uint8_t	 bpp;
	uint8_t	 type_fb;
	uint8_t	 red_mask_size;
	uint8_t	 red_mask_shift;
	uint8_t	 green_mask_size;
	uint8_t	 green_mask_shift;
	uint8_t	 blue_mask_size;
	uint8_t	 blue_mask_shift;
	uint8_t	 reserved[2];
};

/* Multiboot2 tag types. */
#define MULTIBOOT_TAG_TYPE_END	       0
#define MULTIBOOT_TAG_TYPE_FRAMEBUFFER 8

static struct framebuffer_info fb_info = {0};

void
    debug_init (void)
{
	use_debug = true;
}

/* Function to map a physical address to virtual address in page tables. */
static void
    map_framebuffer_address (uint64_t phys_addr)
{
	/* Map the framebuffer to a high virtual address (0xFFFF800000000000 + */
	/* phys_addr) This creates a 1:1 mapping for the framebuffer area. */

	/* kdebugf("Mapping framebuffer address 0x%llx to virtual address */
	/* 0x%llx\n", */
	/*        phys_addr, */
	/*        0xFFFF800000000000 + phys_addr); */

	/* For simplicity, we'll just update the framebuffer address to use the */
	/* virtual mapping. Since we already set up the high virtual address */
	/* mapping in boot.asm, we can access the framebuffer through the high */
	/* virtual address. */
	/* Use the higher-half direct mapping (0xFFFF800000000000 + phys_addr) */
	fb_info.addr = 0xFFFF800000000000ULL + phys_addr;

	kdebugf("Framebuffer mapped to virtual address 0x%llx\n", fb_info.addr);
}

static void
    parse_multiboot_info (void *mb_info)
{
	if ( mb_info == NULL )
	{
		kduts("mb_info is NULL!");
		return;
	}

	uint32_t total_size	 = *(uint32_t *) mb_info;
	uint8_t *current_tag_ptr = (uint8_t *) ((uintptr_t) mb_info + 8);
	uint8_t *end_of_tags	 = (uint8_t *) ((uintptr_t) mb_info + total_size);

	kduts("Parsing multiboot info...");

	while ( current_tag_ptr < end_of_tags )
	{
		struct multiboot_tag *tag = (struct multiboot_tag *) current_tag_ptr;

		uint32_t tag_size = tag->size;
		if ( tag_size == 0 )
		{
			kduts("Error: Invalid tag size 0.");
			return;
		}

		switch ( tag->type )
		{
			case MULTIBOOT_TAG_TYPE_END:
				kduts("Found end tag. Multiboot parsing "
				      "complete.");
				return;

			case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
			{
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
				 * But for the love of god, DO NOT FUCKING
				 * ENABLE THIS! */
				break;
		}

		/* Move to the next tag, ensuring 8-byte alignment. */
		current_tag_ptr += (tag_size + 7) & 0xFFFFFFF8;
	}

	kduts("Finished parsing multiboot tags (end tag not found, but reached "
	      "end of info).");
}

void
    kernel_main (void *mb_info)
{
	/* In this case, we must use use_debug instead of the functions that */
	/* check debug. */
	/* Initialize crucial parts first. The IDT must be loaded before */
	/* any hardware is touched to prevent triple faults. */
	idt_init();
	serial_init();

	if ( mb_info == NULL )
	{
		/*
		 * We can't do anything without multiboot info.
		 * We can't even print an error.
		 * There is nothing we can do.
		 */
		__asm__("cli; hlt");
	}

	/* Parse multiboot information. */
	parse_multiboot_info(mb_info);

	/* Initialize framebuffer if available. */
	if ( fb_info.addr != 0 )
	{
		video_init(&fb_info);
	}
	else
	{
		/* No framebuffer, no visual output possible. */
		kwarn("No framebuffer found", "continuing without visual output");
	}

	debug_init();

	/* Initialize memory manager */
	memory_init(mb_info);
	init_memory_pools();

	/* Initialize ATA and mount EXT2 disk */
	ext2_set_block_device(ata_pio_read, NULL);
	if ( ext2_mount(0) != 0 )
	{
		kputs("EXT2 mount failed\n");
	}

	/* Initialize CPU brand string */
	cpu_init_brand();

	/* Initialize keyboard before enabling interrupts */
	keyboard_init();

	/* Enable interrupts now that all basic drivers are loaded. */
	__asm__ volatile("sti");

	shell();

	/* Main system loop with device polling */
	while ( 1 )
	{
		/* Halt until next interrupt */
		__asm__("hlt");
	}
}
