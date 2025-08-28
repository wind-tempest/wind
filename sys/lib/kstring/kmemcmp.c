// SPDX-License-Identifier: LSL-1.4
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <lib/kstdio/kstddef.h>
#include <lib/kstring/kmemcmp.h>

int
    kmemcmp (const void *s1, const void *s2, ksize_t n) {
	const unsigned char *a = s1;
	const unsigned char *b = s2;
	for (ksize_t i = 0; i < n; ++i) {
		if (a[i] != b[i])
			return (int) a[i] - (int) b[i];
	}
	return 0;
}

