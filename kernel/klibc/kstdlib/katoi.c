// katoi.c

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.infernointeractive.win>
 *
 * This file is part of the Wind Operating System and Tempest Kernel.
 * It is licensed under the GNU General Public License version 3.0  (GPLv3).
 */

#include <wt/kgeneral/kctype/kctype.h>
#include <wt/kgeneral/kerrno/kerrno.h>
#include <wt/klimits.h>
#include <wt/kstdlib/katoi.h>
#include <wt/kstdlib/kstdlib.h>

int
    katoi (const char *s) {
	long result = 0;
	int  sign   = 1;

	kerrno = 0;

	while ( kisspace(*s) )
		s++;

	if ( *s == '+' || *s == '-' ) {
		if ( *s == '-' )
			sign = -1;
		s++;
	}

	if ( !kisdigit(*s) ) {
		kerrno = KEINVAL;
		return 0;
	}

	while ( kisdigit(*s) ) {
		int digit = *s - '0';

		if ( result > (KLONG_MAX - digit) / 10 ) {
			kerrno = KERANGE;
			return sign == 1 ? KINT_MAX : KINT_MIN;
		}

		result = result * 10 + digit;
		s++;
	}

	result *= sign;

	if ( result < KINT_MIN || result > KINT_MAX ) {
		kerrno = KERANGE;
		return result > 0 ? KINT_MAX : KINT_MIN;
	}

	return (int) result;
}
