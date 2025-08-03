// kstrncpy.c

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.tempestfoundation.org>
 *
 * This file is part of the Wind (Operating System) and Tempest (The Kernel).
 * It is licensed under the GNU General Public License version 3.0  (GPLv3).
 */

#include <wt/kstdio/kstddef.h>
#include <wt/kstring/kstrncpy.h>

char *
    kstrncpy (char *dest, const char *src, ksize_t n) {
	char	   *d = dest;
	const char *s = src;

	while ( n && *s ) {
		*d++ = *s++;
		n--;
	}

	while ( n-- ) {
		*d++ = '\0';
	}

	return dest;
}
