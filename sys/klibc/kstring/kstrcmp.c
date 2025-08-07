// kstrcmp.c

// SPDX-License-Identifier: LSL-1.2-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <wt/kstring/kstrcmp.h>

int
    kstrcmp (const char *s1, const char *s2) {
	while ( *s1 && (*s1 == *s2) ) {
		s1++;
		s2++;
	}
	return *(const unsigned char *) s1 - *(const unsigned char *) s2;
}

