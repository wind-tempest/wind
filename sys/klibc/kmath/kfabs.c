// kfabs.c

// SPDX-License-Identifier: LSL-1.2-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <tempest/kmath/kfabs.h>
#include <tempest/kmath/kmath.h>

double
    kfabs (double x) {
	return x < 0.0 ? -x : x;
}
