// kstrtok.c

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

#include "kstrtok.h"

#include "kstddef.h"
#include "kstrchr.h"

static char *strtok_save = KNULL;

char *
    kstrtok (char *str, const char *delim) {
	if ( str != KNULL ) {
		strtok_save = str;
	} else if ( strtok_save == KNULL ) {
		return KNULL;
	}

	// Skip leading delimiters
	while ( *strtok_save && kstrchr(delim, *strtok_save) ) {
		strtok_save++;
	}

	if ( *strtok_save == '\0' ) {
		strtok_save = KNULL;
		return KNULL;
	}

	char *token_start = strtok_save;

	// Find end of token
	while ( *strtok_save && !kstrchr(delim, *strtok_save) ) {
		strtok_save++;
	}

	if ( *strtok_save != '\0' ) {
		*strtok_save = '\0';
		strtok_save++;
	} else {
		strtok_save = KNULL;
	}

	return token_start;
}
