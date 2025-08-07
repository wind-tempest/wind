// kstrtok.c

// SPDX-License-Identifier: LSL-1.2-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
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

