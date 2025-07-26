// kstrchr.c

// SPDX-License-Identifier: LSL-1.0
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.infernointeractive.win>
 *
 * This file is part of the Wind Operating System and Tempest Kernel.
 * It is licensed under the Liberty Software License 1.0 (LSL-1.0).
 * For more information, see <https://wind.infernointeractive.win>.
 */

#include "kstrchr.h"

#include "kstddef.h"

char *
    kstrchr (const char *str, int c) {
	while ( *str != '\0' ) {
		if ( *str == (char) c ) {
			return (char *) str;
		}
		str++;
	}

	if ( c == '\0' ) {
		return (char *) str;
	}

	return KNULL;
}
