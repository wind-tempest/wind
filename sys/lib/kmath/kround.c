// SPDX-License-Identifier: LSL-1.2-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <tempest/klibc/kmath/kmath.h>
#include <tempest/klibc/kmath/kround.h>

double
    kround (double x) {
	if (x >= 0.0) {
		return kfloor(x + 0.5);
	} else {
		return kceil(x - 0.5);
	}
}
