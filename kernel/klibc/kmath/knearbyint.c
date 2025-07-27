// knearbyint.c

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

#include <wt/kmath/kmath.h>
#include <wt/kmath/knearbyint.h>

double
    knearbyint (double x) {
	// Handle special cases
	if ( kisnan(x) )
		return knan("");
	if ( kisinf(x) )
		return x;

	// Round to nearest integer (same as rint for this implementation)
	if ( x >= 0.0 ) {
		double frac = x - kfloor(x);
		if ( frac < 0.5 )
			return kfloor(x);
		else
			return kceil(x);
	} else {
		double frac = kceil(x) - x;
		if ( frac < 0.5 )
			return kceil(x);
		else
			return kfloor(x);
	}
}
