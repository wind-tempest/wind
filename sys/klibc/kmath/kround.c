// kround.c

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <wt/kmath/kmath.h>
#include <wt/kmath/kround.h>

double
    kround (double x)
{
	if ( x >= 0.0 )
	{
		return kfloor(x + 0.5);
	}
	else
	{
		return kceil(x - 0.5);
	}
}
