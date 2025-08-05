// kfabs.c

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <wt/kmath/kfabs.h>
#include <wt/kmath/kmath.h>

double
    kfabs (double x) {
	return x < 0.0 ? -x : x;
}
