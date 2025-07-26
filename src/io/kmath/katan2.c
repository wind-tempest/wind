// katan2.c

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

#include "katan2.h"

#include "kmath.h"

double
    katan2 (double y, double x) {
	// Handle special cases
	if ( kisnan(x) || kisnan(y) )
		return knan("");
	if ( kisinf(x) && kisinf(y) )
		return knan("");

	// If x is zero
	if ( x == 0.0 ) {
		if ( y > 0.0 )
			return K_M_PI_2;
		if ( y < 0.0 )
			return -K_M_PI_2;
		return 0.0; /* y is also zero */
	}

	// If y is zero
	if ( y == 0.0 ) {
		if ( x > 0.0 )
			return 0.0;
		if ( x < 0.0 )
			return K_M_PI;
		return 0.0;
	}

	// If x is infinite
	if ( kisinf(x) ) {
		if ( x > 0.0 )
			return y > 0.0 ? 0.0 : -0.0;
		else
			return y > 0.0 ? K_M_PI : -K_M_PI;
	}

	// If y is infinite
	if ( kisinf(y) ) {
		return y > 0.0 ? K_M_PI_2 : -K_M_PI_2;
	}

	// Normal case: use atan(y/x) with proper quadrant adjustment
	double result = katan(y / x);

	if ( x < 0.0 ) {
		if ( y >= 0.0 )
			result += K_M_PI;
		else
			result -= K_M_PI;
	}

	return result;
}
