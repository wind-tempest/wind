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
    kmemcpy (void *restrict dest, const void *restrict src, ksize_t n) {
	if ( !dest || !src || n == 0 )
		return dest;
	unsigned char	    *d = (unsigned char *) dest;
	const unsigned char *s = (const unsigned char *) src;

	if ( s < d && s + n > d ) {
		for ( ksize_t i = n; i > 0; --i )
			d[i - 1] = s[i - 1];
	} else {
		for ( ksize_t i = 0; i < n; ++i )
			d[i] = s[i];
	}
	return dest;
}
