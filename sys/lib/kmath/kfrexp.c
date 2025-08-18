// SPDX-License-Identifier: LSL-1.3
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <tempest/klibc/kmath/kfrexp.h>
#include <tempest/klibc/kmath/kmath.h>

double
    kfrexp (double x, int *exp) {
	// Handle special cases
	if (kisnan(x)) {
		*exp = 0;
		return knan("");
	}
	if (kisinf(x)) {
		*exp = 0;
		return x;
	}
	if (x == 0.0) {
		*exp = 0;
		return 0.0;
	}

	// Extract exponent using log2
	*exp = (int) kfloor(klog2(kfabs(x))) + 1;

	// Calculate mantissa: x / 2^exp
	return x / kpow(2.0, *exp);
}

