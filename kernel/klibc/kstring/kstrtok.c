// kstrtok.c

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.tempestfoundation.org>
 *
 * This file is part of the Wind (Operating System) and Tempest (The Kernel).
 * It is licensed under the GNU General Public License version 3.0  (GPLv3).
 */

#include <wt/kstdio/kstddef.h>
#include <wt/kstring/kstrchr.h>
#include <wt/kstring/kstrtok.h>

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
