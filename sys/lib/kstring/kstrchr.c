// SPDX-License-Identifier: LSL-1.2-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <tempest/klibc/kstdio/kstddef.h>
#include <tempest/klibc/kstring/kstrchr.h>

char *
    kstrchr (const char *str, int c) {
	while (*str != '\0') {
		if (*str == (char) c) {
			return (char *) str;
		}
		str++;
	}

	if (c == '\0') {
		return (char *) str;
	}

	return KNULL;
}
