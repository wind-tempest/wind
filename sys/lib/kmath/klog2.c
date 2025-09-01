// SPDX-License-Identifier: LSL-1.4
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <lib/kmath/klog2.h>
#include <lib/kmath/kmath.h>

double
    klog2 (double x)
{
	// Handle special cases
	if (kisnan(x))
		return knan("");
	if (x <= 0.0)
		return knan("");

	// Use the relationship: log2(x) = ln(x) / ln(2)
	return klog(x) / K_M_LN2;
}
