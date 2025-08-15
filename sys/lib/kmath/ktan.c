// ktan.c

// SPDX-License-Identifier: LSL-1.2-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <tempest/klibc/kmath/kmath.h>
#include <tempest/klibc/kmath/ktan.h>

double
    ktan (double x) {
	double cos_x = kcos(x);
	if ( cos_x == 0.0 ) {
		// Return infinity for undefined values
		return 1.0 / 0.0;
	}
	return ksin(x) / cos_x;
}
