// SPDX-License-Identifier: LSL-1.4
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <lib/kmath/khypot.h>
#include <lib/kmath/kmath.h>

double
    khypot (double x, double y)
{
	// Handle special cases
	if (kisinf(x) || kisinf(y))
	{
		return 1.0 / 0.0;  // Return infinity
	}

	if (kisnan(x) || kisnan(y))
	{
		return 0.0;  // Return NaN
	}

	// Use the formula: ksqrt(x^2 + y^2)
	return ksqrt(x * x + y * y);
}
