// kfrexp.c

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.tempestfoundation.org>
 *
 * This file is part of the Wind (Operating System) and Tempest (The Kernel).
 * It is licensed under the GNU General Public License version 3.0  (GPLv3).
 */

#include <wt/kmath/kfrexp.h>
#include <wt/kmath/kmath.h>

double
    kfrexp (double x, int *exp) {
	// Handle special cases
	if ( kisnan(x) ) {
		*exp = 0;
		return knan("");
	}
	if ( kisinf(x) ) {
		*exp = 0;
		return x;
	}
	if ( x == 0.0 ) {
		*exp = 0;
		return 0.0;
	}

	// Extract exponent using log2
	*exp = (int) kfloor(klog2(kfabs(x))) + 1;

	// Calculate mantissa: x / 2^exp
	return x / kpow(2.0, *exp);
}
