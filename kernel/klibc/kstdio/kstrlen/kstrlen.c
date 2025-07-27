// kstrlen.c

// SPDX-License-Identifier: LSL-1.2
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.infernointeractive.win>
 *
 * This file is part of the Wind Operating System and Tempest Kernel.
 * It is licensed under the Liberty Software License 1.2 (LSL-1.2).
 * For more information, see <https://wind.infernointeractive.win>.
 */

#include <wt/kstdio/kstrlen/kstrlen.h>

#include <wt/kstdio/kstddef.h>

ksize_t
    kstrlen (const char *str) {
	if ( !str )
		return 0;

	const char *s = str;
	while ( *s )
		s++;
	return (ksize_t) (s - str);
}
