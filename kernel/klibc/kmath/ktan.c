// ktan.c

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

#include <wt/kmath/kmath.h>
#include <wt/kmath/ktan.h>

double
    ktan (double x) {
	double cos_x = kcos(x);
	if ( cos_x == 0.0 ) {
		// Return infinity for undefined values
		return 1.0 / 0.0;
	}
	return ksin(x) / cos_x;
}
