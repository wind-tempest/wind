// keyboard.c

// SPDX-License-Identifier: LSL-1.2
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.infernointeractive.win>
 *
 * This file is part of the Wind Operating System and Tempest Kernel.
 * It is licensed under the Liberty Software License 1.2 (LSL-1.2).
 * For more information, see <https://wind.infernointeractive.win>.
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
