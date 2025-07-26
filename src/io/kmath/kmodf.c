// kmodf.c

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

#include "kmodf.h"

#include "kmath.h"

double
    kmodf (double x, double *iptr) {
	// Handle special cases
	if ( kisnan(x) ) {
		*iptr = knan("");
		return knan("");
	}
	if ( kisinf(x) ) {
		*iptr = x;
		return 0.0;
	}

	// Extract integer part
	if ( x >= 0.0 )
		*iptr = kfloor(x);
	else
		*iptr = kceil(x);

	// Return fractional part
	return x - *iptr;
}
