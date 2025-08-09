// kacos.c

// SPDX-License-Identifier: LSL-1.2-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <tempest/kmath/kacos.h>
#include <tempest/kmath/kmath.h>

double
    kacos (double x) {
	// Check for invalid input
	if ( x < -1.0 || x > 1.0 )
		return knan("");

	// Use the relationship: acos(x) = π/2 - asin(x)
	return K_M_PI_2 - kasin(x);
}
