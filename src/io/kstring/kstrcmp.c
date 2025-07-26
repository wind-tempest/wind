// kstrcmp.c

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

#include "kstrcmp.h"

int
    kstrcmp (const char *s1, const char *s2) {
	while ( *s1 && (*s1 == *s2) ) {
		s1++;
		s2++;
	}
	return *(const unsigned char *) s1 - *(const unsigned char *) s2;
}
