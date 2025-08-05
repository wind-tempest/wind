// kmempcpy.c

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <wt/kstdio/kstddef.h>
#include <wt/kstring/kmempcpy.h>

void *
    kmempcpy (void *dest, const void *src, ksize_t n)
{
	char	   *d = (char *) dest;
	const char *s = (const char *) src;
	for ( ksize_t i = 0; i < n; i++ )
		d[i] = s[i];
	return d + n;
}
