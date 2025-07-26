// kstrrchr.c

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

#include "kstrrchr.h"

#include "kstddef.h"

const char *
    kstrrchr (const char *str, int c) {
	if ( !str )
		return KNULL;

	const char  ch	 = (char) c;
	const char *last = KNULL;

	do {
		if ( *str == ch )
			last = str;
	} while ( *str++ );

	return last;
}
