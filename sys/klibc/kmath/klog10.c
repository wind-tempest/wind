// klog10.c

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <wt/kmath/klog10.h>
#include <wt/kmath/kmath.h>

double
    klog10 (double x)
{
	// Handle special cases
	if ( kisnan(x) )
		return knan("");
	if ( x <= 0.0 )
		return knan("");

	// Use the relationship: log10(x) = ln(x) / ln(10)
	return klog(x) / klog(10);
}
