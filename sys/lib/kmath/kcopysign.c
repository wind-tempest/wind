// SPDX-License-Identifier: LSL-1.4
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <lib/kmath/kcopysign.h>
#include <lib/kmath/kmath.h>

double
    kcopysign (double x, double y) {
	if (y >= 0.0) {
		return kfabs(x);
	} else {
		return -kfabs(x);
	}
}

