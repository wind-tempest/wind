// kmemcmp.c

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.infernointeractive.win>
 *
 * This file is part of the Wind Operating System and Tempest Kernel.
 * It is licensed under the GNU General Public License version 3.0  (GPLv3).
 */

#include <wt/kstring/kmemcmp.h>

int
    kmemcmp (const void *s1, const void *s2, unsigned long n) {
	const unsigned char *a = s1;
	const unsigned char *b = s2;
	for ( unsigned long i = 0; i < n; ++i ) {
		if ( a[i] != b[i] )
			return a[i] - b[i];
	}
	return 0;
}
