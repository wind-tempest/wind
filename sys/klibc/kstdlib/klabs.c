// klabs.c

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <wt/kstdlib/klabs.h>
#include <wt/kstdlib/kstdlib.h>

long
    labs (long a)
{
	return a > 0 ? a : -a;
}
