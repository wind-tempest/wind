// kstrcat.c

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.infernointeractive.win>
 *
 * This file is part of the Wind Operating System and Tempest Kernel.
 * It is licensed under the GNU General Public License version 3.0  (GPLv3).
 */

#include <wt/kstring/kstrcat.h>

char *
    kstrcat (char *dest, const char *src) {
	if ( !dest || !src )
		return dest;

	// Find the end of dest
	char *end = dest;
	while ( *end ) {
		end++;
	}

	// Copy src to the end of dest
	while ( *src ) {
		*end = *src;
		end++;
		src++;
	}

	*end = '\0';
	return dest;
}
