// SPDX-License-Identifier: LSL-1.4
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <lib/kmath/kfabs.h>
#include <lib/kmath/kmath.h>

double
    kfabs (double x)
{
	return x < 0.0 ? -x : x;
}
