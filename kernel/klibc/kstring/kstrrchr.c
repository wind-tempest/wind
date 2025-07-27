// kstrrchr.c

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
#include <wt/kstring/kstrrchr.h>

const char *
    kstrrchr (const char *str, int c) {
	if ( !str )
		return KNULL;

	const char  ch	 = (char) c;
	const char *last = KNULL;

	do {
		if ( *str == ch )
			last = str;
	} while ( *str++ );

	return last;
}
