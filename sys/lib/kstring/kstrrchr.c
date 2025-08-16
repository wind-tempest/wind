// SPDX-License-Identifier: LSL-1.2-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <tempest/klibc/kstdio/kstddef.h>
#include <tempest/klibc/kstring/kstrrchr.h>

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

