// kisnan.c

// SPDX-License-Identifier: LSL-1.2-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <tempest/kmath/kisnan.h>
#include <tempest/kmath/kmath.h>

int
    kisnan (double x) {
	return x != x;
}
