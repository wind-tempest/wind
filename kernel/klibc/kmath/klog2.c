// klog2.c

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.tempestfoundation.org>
 *
 * This file is part of the Wind (Operating System) and Tempest (The Kernel).
 * It is licensed under the GNU General Public License version 3.0  (GPLv3).
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
