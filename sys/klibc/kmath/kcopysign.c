// kcopysign.c

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <wt/kmath/kcopysign.h>
#include <wt/kmath/kmath.h>

double
    kcopysign (double x, double y)
{
	if ( y >= 0.0 )
		{
			return kfabs(x);
		}
	else
		{
			return -kfabs(x);
		}
}
