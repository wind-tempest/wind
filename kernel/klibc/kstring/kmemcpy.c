// kmemcpy.c

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

#include <wt/kstdio/kstddef.h>
#include <wt/kstring/kmemcpy.h>

void *
    kmemcpy (void *dest, const void *src, ksize_t n) {
	unsigned char	    *d = dest;
	const unsigned char *s = src;
	for ( ksize_t i = 0; i < n; ++i )
		d[i] = s[i];
	return dest;
}
