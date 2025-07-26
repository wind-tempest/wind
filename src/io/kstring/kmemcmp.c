// kmemcmp.c

// SPDX-License-Identifier: LSL-1.1
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.infernointeractive.win>
 *
 * This file is part of the Wind Operating System and Tempest Kernel.
 * It is licensed under the Liberty Software License 1.1 (LSL-1.1).
 * For more information, see <https://wind.infernointeractive.win>.
 */

#include "kmemcmp.h"

int
    kmemcmp (const void *s1, const void *s2, unsigned long n) {
	const unsigned char *a = (const unsigned char *) s1;
	const unsigned char *b = (const unsigned char *) s2;
	for ( unsigned long i = 0; i < n; ++i ) {
		if ( a[i] != b[i] )
			return (int) a[i] - (int) b[i];
	}
	return 0;
}
