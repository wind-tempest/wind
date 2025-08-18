// SPDX-License-Identifier: LSL-1.3
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <tempest/klibc/kmath/kisfinite.h>
#include <tempest/klibc/kmath/kmath.h>

int
    kisfinite (double x) {
	return !kisnan(x) && !kisinf(x);
}

