// kround.c

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

#include <wt/kmath/kround.h>

#include <wt/kmath/kmath.h>

double
    kround (double x) {
	if ( x >= 0.0 ) {
		return kfloor(x + 0.5);
	} else {
		return kceil(x - 0.5);
	}
}
