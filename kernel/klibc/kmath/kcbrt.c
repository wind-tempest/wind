// kcbrt.c

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.infernointeractive.win>
 *
 * This file is part of the Wind Operating System and Tempest Kernel.
 * It is licensed under the GNU General Public License version 3.0  (GPLv3).
 */

#include <wt/kmath/kcbrt.h>
#include <wt/kmath/kmath.h>

double
    kcbrt (double x) {
	// Handle special cases
	if ( kisnan(x) )
		return knan("");
	if ( kisinf(x) )
		return x;
	if ( x == 0.0 )
		return 0.0;

	// Use Newton's method for cube root
	double y = kfabs(x);
	double guess;

	// Initial guess
	if ( y > 1.0 )
		guess = y / 3.0;
	else
		guess = y * 2.0 / 3.0;

	// Newton iteration: x_{n+1} = (2*x_n + y/x_n^2) / 3
	for ( int i = 0; i < 10; i++ ) {
		double guess2	 = guess * guess;
		double new_guess = (2.0 * guess + y / guess2) / 3.0;
		if ( kfabs(new_guess - guess) < 1e-15 )
			break;
		guess = new_guess;
	}

	return x < 0.0 ? -guess : guess;
}
