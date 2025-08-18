// SPDX-License-Identifier: LSL-1.3
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <lib/kmath/kmath.h>
#include <lib/kmath/kround.h>

double
    kround (double x) {
	if (x >= 0.0) {
		return kfloor(x + 0.5);
	} else {
		return kceil(x - 0.5);
	}
}
