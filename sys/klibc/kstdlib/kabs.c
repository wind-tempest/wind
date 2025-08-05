// kabs.c

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <wt/kstdlib/kabs.h>
#include <wt/kstdlib/kstdlib.h>

int
    kabs (int a) {
	return a > 0 ? a : -a;
}
