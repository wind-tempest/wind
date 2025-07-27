// katan.c

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

#include "katan.h"

#include "kmath.h"

double
    katan (double x) {
	// Handle special cases
	if ( kisnan(x) )
		return knan("");
	if ( kisinf(x) )
		return x > 0 ? K_M_PI_2 : -K_M_PI_2;
	if ( x == 0.0 )
		return 0.0;

	// Use series expansion for small values
	if ( kfabs(x) < 1.0 ) {
		double x2 = x * x;
		double x3 = x2 * x;
		double x5 = x3 * x2;
		double x7 = x5 * x2;
		double x9 = x7 * x2;
		return x - x3 / 3.0 + x5 / 5.0 - x7 / 7.0 + x9 / 9.0;
	}

	// For large values, use atan(x) = π/2 - atan(1/x)
	if ( x > 0 )
		return K_M_PI_2 - katan(1.0 / x);
	else
		return -K_M_PI_2 - katan(1.0 / x);
}
