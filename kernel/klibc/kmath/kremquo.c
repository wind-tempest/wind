// kremquo.c

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.infernointeractive.win>
 *
 * This file is part of the Wind Operating System and Tempest Kernel.
 * It is licensed under the GNU General Public License version 3.0  (GPLv3).
 */

#include <wt/kmath/kmath.h>
#include <wt/kmath/kremquo.h>

double
    kremquo (double x, double y, int *quo) {
	// Handle special cases
	if ( kisnan(x) || kisnan(y) ) {
		*quo = 0;
		return knan("");
	}
	if ( kisinf(x) ) {
		*quo = 0;
		return knan("");
	}
	if ( y == 0.0 ) {
		*quo = 0;
		return knan("");
	}
	if ( kisinf(y) ) {
		*quo = 0;
		return x;
	}

	// Calculate quotient
	*quo = (int) kround(x / y);

	// Calculate remainder
	double r = x - *quo * y;

	// Adjust to get remainder in the range [-|y|/2, |y|/2]
	if ( kfabs(r) > kfabs(y) / 2.0 ) {
		if ( r > 0 ) {
			r -= kfabs(y);
			(*quo)++;
		} else {
			r += kfabs(y);
			(*quo)--;
		}
	}

	return r;
}
