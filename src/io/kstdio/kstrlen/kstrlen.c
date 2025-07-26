// kstrlen.c

// SPDX-License-Identifier: LSL-1.1
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.infernointeractive.win>
 *
 * This file is part of the Wind Operating System and Tempest Kernel.
 * It is licensed under the Liberty Software License 1.1 (LSL-1.1).
 * For more information, see <https://wind.infernointeractive.win>.
 */

// SPDX-License-Identifier: LSL-1.0

#include "kstrlen.h"

#include "kstddef.h"

ksize_t
    kstrlen (const char *str) {
	if ( !str )
		return 0;

	const char *s = str;
	while ( *s )
		s++;
	return (ksize_t) (s - str);
}
