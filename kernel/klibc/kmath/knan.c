// knan.c

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

#include <wt/kmath/knan.h>

#include <wt/kmath/kmath.h>

double
    knan (const char *tagp) {
	// Return NaN - the tagp parameter is ignored in this implementation
	(void) tagp; // Suppress unused parameter warning

	// Return a quiet NaN
	union {
		double		   d;
		unsigned long long u;
	} u;

	u.u = 0x7ff8000000000000ULL; // Quiet NaN
	return u.d;
}
