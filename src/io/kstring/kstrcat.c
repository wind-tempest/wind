// kstrcat.c

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

#include "kstrcat.h"

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
