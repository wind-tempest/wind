// kmodf.c

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.infernointeractive.win>
 *
 * This file is part of the Wind Operating System and Tempest Kernel.
 * It is licensed under the GNU General Public License version 3.0  (GPLv3).
 */

#include <wt/kmath/kmath.h>
#include <wt/kmath/kmodf.h>

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
