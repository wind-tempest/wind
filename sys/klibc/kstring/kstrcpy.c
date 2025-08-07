// kstrcpy.c

// SPDX-License-Identifier: LSL-1.2-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <wt/kstdio/kstrlen/kstrlen.h>
#include <wt/kstring/kmemcpy.h>
#include <wt/kstring/kstrcpy.h>

char *
    kstrcpy (char *dest, const char *src) {
	ksize_t len = kstrlen(src);
	kmemcpy(dest, src, len + 1);  // +1 for null terminator
	return dest;
}

