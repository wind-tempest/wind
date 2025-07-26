// kstrncpy.c

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

#include "kstrncpy.h"

#include "kstddef.h"

char *
    kstrncpy (char *dest, const char *src, ksize_t n) {
	ksize_t i;
	for ( i = 0; i < n && src[i] != '\0'; i++ )
		dest[i] = src[i];
	for ( ; i < n; i++ )
		dest[i] = '\0';
	return dest;
}
