// kldexp.c

// SPDX-License-Identifier: LSL-1.2
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.infernointeractive.win>
 *
 * This file is part of the Wind Operating System and Tempest Kernel.
 * It is licensed under the Liberty Software License 1.2 (LSL-1.2).
 * For more information, see <https://wind.infernointeractive.win>.
 */

#include <wt/kmath/kldexp.h>

#include <wt/kmath/kmath.h>

double
    kldexp (double x, int exp) {
	// Handle special cases
	if ( kisnan(x) )
		return knan("");
	if ( kisinf(x) )
		return x;
	if ( x == 0.0 )
		return 0.0;

	// Use the relationship: ldexp(x, exp) = x * 2^exp
	return x * kpow(2.0, exp);
}
