// kstrrchr.c

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <wt/kstdio/kstddef.h>
#include <wt/kstring/kstrrchr.h>

const char *
    kstrrchr (const char *str, int c)
{
	if ( !str )
		return KNULL;

	const char  ch	 = (char) c;
	const char *last = KNULL;

	do
		{
			if ( *str == ch )
				last = str;
		}
	while ( *str++ );

	return last;
}
