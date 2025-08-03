// kstrncmp.c

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.tempestfoundation.org>
 *
 * This file is part of the Wind (Operating System) and Tempest (The Kernel).
 * It is licensed under the GNU General Public License version 3.0  (GPLv3).
 */

#include <wt/kstdio/kstddef.h>
#include <wt/kstring/kstrncmp.h>

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
