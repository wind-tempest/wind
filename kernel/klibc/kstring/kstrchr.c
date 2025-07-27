// kstrchr.c

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
#include <wt/kstring/kstrchr.h>

char *
    kstrchr (const char *str, int c) {
	while ( *str != '\0' ) {
		if ( *str == (char) c ) {
			return (char *) str;
		}
		str++;
	}

	if ( c == '\0' ) {
		return (char *) str;
	}

	return KNULL;
}
