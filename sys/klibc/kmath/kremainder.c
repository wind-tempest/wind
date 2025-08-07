// kremainder.c

// SPDX-License-Identifier: LSL-1.2-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <wt/kmath/kmath.h>
#include <wt/kmath/kremainder.h>

double
    kremainder (double x, double y) {
	// Handle special cases
	if ( kisnan(x) || kisnan(y) )
		return knan("");
	if ( kisinf(x) )
		return knan("");
	if ( y == 0.0 )
		return knan("");
	if ( kisinf(y) )
		return x;

	// Calculate remainder using fmod and adjust for rounding
	double r = kfmod(x, y);

	// Adjust to get remainder in the range [-|y|/2, |y|/2]
	if ( kfabs(r) > kfabs(y) / 2.0 ) {
		if ( r > 0 )
			r -= kfabs(y);
		else
			r += kfabs(y);
	}

	return r;
}

