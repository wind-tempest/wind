// keyboard.c

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

#include "keyboard.h"

#include "core/cpu/idt.h"
#include "ksignal.h"
#include "kstdio.h"

#define KBD_DATA_PORT	0x60
#define KBD_BUFFER_SIZE 256

// Simple keyboard buffer
static unsigned char kbd_buffer[KBD_BUFFER_SIZE];
static int	     kbd_buffer_head = 0;
static int	     kbd_buffer_tail = 0;
static kbool	     shift_pressed   = kfalse;
static kbool	     caps_lock	     = kfalse;

// Scancode set 1 to ASCII mapping for US keyboard layout
unsigned char kbd_us[128] = {
    0,	 27,   '1',  '2', '3',	'4', '5', '6', '7', '8', '9', '0', '-',
    '=', '\b', '\t', 'q', 'w',	'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
    '[', ']',  '\n', 0,	  'a',	's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',
    ';', '\'', '`',  0,	  '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',',
    '.', '/',  0,    '*', 0,	' ', 0,	  0,   0,   0,	 0,   0,   0,
    0,	 0,    0,    0,	  0,	0,   0,	  0,   '-', 0,	 0,   0,   /* + -> */ 0,
    0,	 0,    0,    0,	  0,	0,   0,	  0,   0,   0,
};

static unsigned char kbd_us_shift[128] = {
    0,	 27,   '!',  '@', '#', '$', '%', '^', '&', '*', '(', ')', '_',
    '+', '\b', '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
    '{', '}',  '\n', 0,	  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
    ':', '"',  '~',  0,	  '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<',
    '>', '?',  0,    '*', 0,   ' ', 0,	 0,   0,   0,	0,   0,	  0,
    0,	 0,    0,    0,	  0,   0,   0,	 0,   '-', 0,	0,   0,	  /* + -> */ 0,
    0,	 0,    0,    0,	  0,   0,   0,	 0,   0,   0,
};

static void
    keyboard_handler (registers_t *regs) {
	(void) regs;
	unsigned char scancode = kinb(KBD_DATA_PORT);

	// Caps Lock pressed
	if ( scancode == 0x3A ) {
		caps_lock = !caps_lock; // Toggle caps lock
		return;
	}

	if ( scancode == 0x2A || scancode == 0x36 ) {
		shift_pressed = ktrue;
		return;
	}
	if ( scancode == 0xAA || scancode == 0xB6 ) {
		shift_pressed = kfalse;
		return;
	}

	if ( scancode < 128 ) {
		unsigned char c = shift_pressed ? kbd_us_shift[scancode] : kbd_us[scancode];

		if ( caps_lock && !shift_pressed ) {
			// Caps Lock ON + Shift OFF:
			if ( c >= 'a' && c <= 'z' ) {
				c -= 32;
			}
		} else if ( caps_lock && shift_pressed ) {
			// Caps Lock ON + Shift ON:
			if ( c >= 'A' && c <= 'Z' ) {
				c += 32;
			}
		}

		if ( c != 0 ) {
			if ( (kbd_buffer_head + 1) % KBD_BUFFER_SIZE != kbd_buffer_tail ) {
				kbd_buffer[kbd_buffer_head] = c;
				kbd_buffer_head = (kbd_buffer_head + 1) % KBD_BUFFER_SIZE;
			}
		}
	}
}

int
    getchar (void) {
	// Wait for a character to be available
	while ( kbd_buffer_head == kbd_buffer_tail ) {
		__asm__ volatile("sti; hlt"); // Wait for interrupt
	}
	__asm__ volatile("cli"); // Disable interrupts while reading buffer

	int c		= kbd_buffer[kbd_buffer_tail];
	kbd_buffer_tail = (kbd_buffer_tail + 1) % KBD_BUFFER_SIZE;

	__asm__ volatile("sti"); // Re-enable interrupts
	return c;
}

void
    keyboard_init (void) {
	register_irq_handler(1, keyboard_handler);
}
