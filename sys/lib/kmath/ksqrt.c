// SPDX-License-Identifier: LSL-1.4
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <lib/kmath/kmath.h>
#include <lib/kmath/ksqrt.h>

double
    ksqrt (double x) {
	if (x < 0.0) {
		// Return NaN for negative numbers
		return 0.0 / 0.0;
	}

	if (x == 0.0 || x == 1.0) {
		return x;
	}

	// Newton-Raphson method for square root
	double guess = x / 2.0;
	double prev_guess;

	do {
		prev_guess = guess;
		guess      = (guess + x / guess) / 2.0;
	} while (kfabs(guess - prev_guess) > 1e-15);

	return guess;
}

