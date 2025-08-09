// klabs.c

// SPDX-License-Identifier: LSL-1.2-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <tempest/kstdlib/klabs.h>
#include <tempest/kstdlib/kstdlib.h>

long
    labs (long a) {
	return a > 0 ? a : -a;
}
