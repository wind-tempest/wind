// kstrlen.c

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
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
