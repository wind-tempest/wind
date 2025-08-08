// kisinf.c

// SPDX-License-Identifier: LSL-1.2-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <wt/kmath/kisinf.h>
#include <wt/kmath/kmath.h>

int
    kisinf (double x) {
	return x == 1.0 / 0.0 || x == -1.0 / 0.0;
}
