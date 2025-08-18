// SPDX-License-Identifier: LSL-1.3
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <tempest/klibc/kstdio/kstddef.h>
#include <tempest/klibc/kstdio/kstrlen/kstrlen.h>

ksize_t
    kstrlen (const char *str) {
	if (!str)
		return 0;

	const char *s = str;
	while (*s)
		s++;
	return (ksize_t) (s - str);
}

