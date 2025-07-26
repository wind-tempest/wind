// klog.c

// SPDX-License-Identifier: LSL-1.0
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.infernointeractive.win>
 *
 * This file is part of the Wind Operating System and Tempest Kernel.
 * It is licensed under the Liberty Software License 1.0 (LSL-1.0).
 * For more information, see <https://wind.infernointeractive.win>.
 */

#include "klog.h"

#include "kmath.h"

double
    klog (double x) {
	// Handle special cases
	if ( x <= 0.0 ) {
		// Return NaN for non-positive numbers
		return 0.0 / 0.0;
	}

	if ( x == 1.0 )
		return 0.0;

	// Use Newton's method to find ln(x)
	// We solve: e^y = x, so y = ln(x)
	double y = 0.0;
	double prev_y;

	// Initial guess
	if ( x > 1.0 ) {
		y = 1.0;
	} else {
		y = -1.0;
	}

	// Newton iteration: y_{n+1} = y_n + (x - e^y_n) / e^y_n
	for ( int i = 0; i < 10; i++ ) {
		prev_y	     = y;
		double exp_y = kexp(y);
		y	     = y + (x - exp_y) / exp_y;

		if ( kfabs(y - prev_y) < 1e-15 ) {
			break;
		}
	}

	return y;
}
