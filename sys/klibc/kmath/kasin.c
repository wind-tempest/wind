// kasin.c

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.tempestfoundation.org>
 *
 * This file is part of the Wind (Operating System) and Tempest (The Kernel).
 * It is licensed under the GNU General Public License version 3.0  (GPLv3).
 */

#include <wt/kmath/kasin.h>
#include <wt/kmath/kmath.h>

double
    kasin (double x) {
	// Check for invalid input
	if ( x < -1.0 || x > 1.0 )
		return knan("");

	// Use Newton's method to find arcsin
	if ( kfabs(x) < 0.5 ) {
		double y  = x;
		double y2 = y * y;
		double y3 = y2 * y;
		double y5 = y3 * y2;
		double y7 = y5 * y2;
		return y + y3 / 6.0 + 3.0 * y5 / 40.0 + 5.0 * y7 / 112.0;
	}

	// For values closer to ±1, use different approach
	if ( x > 0 )
		return K_M_PI_2 - kasin(ksqrt(1.0 - x * x));
	else
		return -K_M_PI_2 + kasin(ksqrt(1.0 - x * x));
}
