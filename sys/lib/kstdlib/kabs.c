// SPDX-License-Identifier: LSL-1.3
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <tempest/klibc/kstdlib/kabs.h>
#include <tempest/klibc/kstdlib/kstdlib.h>

int
    kabs (int a) {
	return a > 0 ? a : -a;
}

