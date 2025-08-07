// kinb.c

// SPDX-License-Identifier: LSL-1.2-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <wt/kasm/kinb.h>
#include <wt/kasm/kio.h>

unsigned char
    kinb (unsigned short port) {
	unsigned char ret;
	__asm__ __volatile__("inb %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

