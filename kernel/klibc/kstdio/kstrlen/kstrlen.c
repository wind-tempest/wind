// kstrlen.c

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
#include <wt/kstdio/kstrlen/kstrlen.h>

ksize_t
    kstrlen (const char *str) {
	if ( !str )
		return 0;

	const char *s = str;
	while ( *s )
		s++;
	return (ksize_t) (s - str);
}
