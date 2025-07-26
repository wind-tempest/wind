// ktrunc.c

// SPDX-License-Identifier: LSL-1.0
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.infernointeractive.win>
 *
 * This file is part of the Wind Operating System and Tempest Kernel.
 * It is licensed under the Liberty Software License 1.0 (LSL-1.0).
 * For more information, see <https://wind.infernointeractive.win>.
 */

#include "ktrunc.h"

#include "kmath.h"

double
    ktrunc (double x) {
	// Handle special cases
	if ( kisnan(x) )
		return knan("");
	if ( kisinf(x) )
		return x;

	// For positive numbers, truncate towards zero
	if ( x >= 0.0 )
		return kfloor(x);
	else
		return kceil(x);
}
