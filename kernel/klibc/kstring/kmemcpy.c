// kmemcpy.c

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.infernointeractive.win>
 *
 * This file is part of the Wind Operating System and Tempest Kernel.
 * It is licensed under the GNU General Public License version 3.0  (GPLv3).
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
