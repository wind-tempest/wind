// kfrexp.c

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

#include "kfrexp.h"

#include "kmath.h"

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
