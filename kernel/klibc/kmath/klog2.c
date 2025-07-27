// klog2.c

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

#include <wt/kmath/klog2.h>
#include <wt/kmath/kmath.h>

double
    klog2 (double x) {
	// Handle special cases
	if ( kisnan(x) )
		return knan("");
	if ( x <= 0.0 )
		return knan("");

	// Use the relationship: log2(x) = ln(x) / ln(2)
	return klog(x) / K_M_LN2;
}
