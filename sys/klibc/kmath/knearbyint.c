// knearbyint.c

// SPDX-License-Identifier: LSL-1.2-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <tempest/kmath/kmath.h>
#include <tempest/kmath/knearbyint.h>

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
