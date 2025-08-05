// serial.c

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include "serial.h"

#include <wt/kasm/kio.h>
#include <wt/kstdio/kstdbool.h>
#include <wt/kstdlib/kitoa.h>

#define PORT 0x3f8  // COM1

kbool
    is_serial_available (void);

void
    serial_init (void) {
	if ( !is_serial_available() )
		return;		// Exit if serial not available.
	koutb(PORT + 1, 0x00);	// Disable all interrupts.
	koutb(PORT + 3, 0x80);	// Enable DLAB (set baud rate divisor).
	koutb(PORT + 0, 0x03);	// Set divisor to 3 (lo byte) 38400 baud
	koutb(PORT + 1, 0x00);	//                  (hi byte)
	koutb(PORT + 3, 0x03);	// 8 bits, no parity, one stop bit.
	koutb(PORT + 2,
	      0xC7);		// Enable FIFO, clear them, with 14-byte threshold.
	koutb(PORT + 4, 0x0B);	// IRQs enabled, RTS/DSR set.
}

static int
    is_transmit_empty (void) {
	return kinb(PORT + 5) & 0x20;
}

void
    serial_write (char a) {
	if ( !is_serial_available() || !a )
		return;	 // Exit if serial not available or if char is null.
	while ( is_transmit_empty() == 0 )
		;
	koutb(PORT, (unsigned char) a);
}

void
    serial_writes (const char *s) {
	if ( !is_serial_available() || !s )
		return;	 // Exit if serial not available or if string is null.
	while ( *s ) {
		serial_write(*s++);
	}
}

void
    serial_write_int (int i) {
	if ( !is_serial_available() )
		return;	 // Exit if serial not available.
	char buf[16];
	kitoa(buf, buf + 15, i, 10, 0);
	serial_writes(buf);
}

kbool
    is_serial_available (void) {
	// Save original values.
	unsigned char original_lcr = kinb(PORT + 3);
	unsigned char original_mcr = kinb(PORT + 4);

	// Set up loopback mode.
	koutb(PORT + 3, 0x80);	// Enable DLAB.
	koutb(PORT + 4, 0x10);	// Enable loopback mode.

	// Test value to write and read back.
	unsigned char test_value = 0xAA;

	// Write test value.
	koutb(PORT, test_value);

	// Small delay to ensure data is processed.
	for ( volatile int i = 0; i < 1000; i++ )
		;

	// Read back the value.
	unsigned char read_value = kinb(PORT);

	// Restore original values.
	koutb(PORT + 3, original_lcr);
	koutb(PORT + 4, original_mcr);

	// If read value matches test value, port is available.
	return (read_value == test_value);
}
