// knan.c

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <wt/kmath/kmath.h>
#include <wt/kmath/knan.h>

double
    knan (const char *tagp)
{
	// Return NaN - the tagp parameter is ignored in this implementation
	(void) tagp;  // Suppress unused parameter warning

	// Return a quiet NaN
	union
	{
		double		   d;
		unsigned long long u;
	} u;

	u.u = 0x7ff8000000000000ULL;  // Quiet NaN
	return u.d;
}
