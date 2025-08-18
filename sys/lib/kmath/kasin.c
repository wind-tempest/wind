// SPDX-License-Identifier: LSL-1.3
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <tempest/klibc/kmath/kasin.h>
#include <tempest/klibc/kmath/kmath.h>

double
    kasin (double x) {
	// Check for invalid input
	if (x < -1.0 || x > 1.0)
		return knan("");

	// Use Newton's method to find arcsin
	if (kfabs(x) < 0.5) {
		double y  = x;
		double y2 = y * y;
		double y3 = y2 * y;
		double y5 = y3 * y2;
		double y7 = y5 * y2;
		return y + y3 / 6.0 + 3.0 * y5 / 40.0 + 5.0 * y7 / 112.0;
	}

	// For values closer to ±1, use different approach
	if (x > 0)
		return K_M_PI_2 - kasin(ksqrt(1.0 - x * x));
	else
		return -K_M_PI_2 + kasin(ksqrt(1.0 - x * x));
}

