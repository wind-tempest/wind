// SPDX-License-Identifier: LSL-1.3
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <tempest/klibc/kmath/klog1p.h>
#include <tempest/klibc/kmath/kmath.h>

double
    klog1p (double x) {
	// Handle special cases
	if (kisnan(x))
		return knan("");
	if (x < -1.0)
		return knan("");
	if (x == -1.0)
		return -K_INFINITY;
	if (kisinf(x))
		return x > 0 ? K_INFINITY : knan("");

	// For small values, use Taylor series to avoid loss of precision
	if (kfabs(x) < 0.1) {
		double x2 = x * x;
		double x3 = x2 * x;
		double x4 = x3 * x;
		double x5 = x4 * x;
		return x - x2 / 2.0 + x3 / 3.0 - x4 / 4.0 + x5 / 5.0;
	}

	// For larger values, use log(1 + x)
	return klog(1.0 + x);
}

