// khypot.c

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.infernointeractive.win>
 *
 * This file is part of the Wind Operating System and Tempest Kernel.
 * It is licensed under the GNU General Public License version 3.0  (GPLv3).
 */

#include <wt/kmath/khypot.h>
#include <wt/kmath/kmath.h>

double
    khypot (double x, double y) {
	// Handle special cases
	if ( kisinf(x) || kisinf(y) ) {
		return 1.0 / 0.0; // Return infinity
	}

	if ( kisnan(x) || kisnan(y) ) {
		return 0.0 / 0.0; // Return NaN
	}

	// Use the formula: ksqrt(x^2 + y^2)
	return ksqrt(x * x + y * y);
}
