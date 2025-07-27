// kexp.c

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.infernointeractive.win>
 *
 * This file is part of the Wind Operating System and Tempest Kernel.
 * It is licensed under the GNU General Public License version 3.0  (GPLv3).
 */

#include <wt/kmath/kexp.h>
#include <wt/kmath/kmath.h>

double
    kexp (double x) {
	// Handle special cases
	if ( x == 0.0 )
		return 1.0;
	if ( x < -700.0 )
		return 0.0; // Underflow
	if ( x > 700.0 )
		return 1.0 / 0.0; // Overflow

	// Use Taylor series: e^x = 1 + x + x^2/2! + x^3/3! + ...
	double result	 = 1.0;
	double term	 = 1.0;
	double factorial = 1.0;

	for ( int i = 1; i <= 20; i++ ) {
		term *= x;
		factorial *= i;
		result += term / factorial;
	}

	return result;
}
