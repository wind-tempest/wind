// ksqrt.c

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
#include <wt/kmath/ksqrt.h>

double
    ksqrt (double x) {
	if ( x < 0.0 ) {
		// Return NaN for negative numbers
		return 0.0 / 0.0;
	}

	if ( x == 0.0 || x == 1.0 ) {
		return x;
	}

	// Newton-Raphson method for square root
	double guess = x / 2.0;
	double prev_guess;

	do {
		prev_guess = guess;
		guess	   = (guess + x / guess) / 2.0;
	} while ( kfabs(guess - prev_guess) > 1e-15 );

	return guess;
}
