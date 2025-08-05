// koutw.c

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <wt/kasm/kio.h>
#include <wt/kasm/koutw.h>

void
    koutw (unsigned short port, unsigned short val) {
	__asm__ __volatile__("outw %0, %1" : : "a"(val), "Nd"(port));
}
