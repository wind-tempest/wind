// SPDX-License-Identifier: LSL-1.2-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <tempest/klibc/kstring/kstrcat.h>

char *
    kstrcat (char *dest, const char *src) {
	if (!dest || !src)
		return dest;

	// Find the end of dest
	char *end = dest;
	while (*end) {
		end++;
	}

	// Copy src to the end of dest
	while (*src) {
		*end = *src;
		end++;
		src++;
	}

	*end = '\0';
	return dest;
}
