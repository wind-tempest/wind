// kstrncmp.c

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

#include <wt/kstring/kstrncmp.h>

#include <wt/kstdio/kstddef.h>

int
    kstrncmp (const char *s1, const char *s2, ksize_t n) {
	for ( ksize_t i = 0; i < n; i++ ) {
		if ( s1[i] != s2[i] ) {
			return (unsigned char) s1[i] - (unsigned char) s2[i];
		}
		if ( s1[i] == '\0' ) {
			return 0;
		}
	}
	return 0;
}
