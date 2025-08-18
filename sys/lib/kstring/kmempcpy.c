// SPDX-License-Identifier: LSL-1.3
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <tempest/klibc/kstdio/kstddef.h>
#include <tempest/klibc/kstring/kmempcpy.h>

void *
    kmempcpy (void *dest, const void *src, ksize_t n) {
	char       *d = (char *) dest;
	const char *s = (const char *) src;
	for (ksize_t i = 0; i < n; i++)
		d[i] = s[i];
	return d + n;
}

