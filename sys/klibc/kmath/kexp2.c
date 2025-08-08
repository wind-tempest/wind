// kexp2.c

// SPDX-License-Identifier: LSL-1.2-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <wt/kmath/kexp2.h>
#include <wt/kmath/kmath.h>

double
    kexp2 (double x) {
	// Handle special cases
	if ( kisnan(x) )
		return knan("");
	if ( kisinf(x) )
		return x > 0 ? K_INFINITY : 0.0;

	// Use the relationship: exp2(x) = exp(x * ln(2))
	return kexp(x * K_M_LN2);
}
