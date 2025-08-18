// SPDX-License-Identifier: LSL-1.3
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <lib/kmath/kmath.h>
#include <lib/kmath/kmodf.h>

double
    kmodf (double x, double *iptr) {
	// Handle special cases
	if (kisnan(x)) {
		*iptr = knan("");
		return knan("");
	}
	if (kisinf(x)) {
		*iptr = x;
		return 0.0;
	}

	// Extract integer part
	if (x >= 0.0)
		*iptr = kfloor(x);
	else
		*iptr = kceil(x);

	// Return fractional part
	return x - *iptr;
}
