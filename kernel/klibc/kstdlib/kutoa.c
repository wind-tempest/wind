// kutoa.c

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.tempestfoundation.org>
 *
 * This file is part of the Wind (Operating System) and Tempest (The Kernel).
 * It is licensed under the GNU General Public License version 3.0  (GPLv3).
 */

#include <wt/kgeneral/kcharset/kcharset.h>
#include <wt/kstdlib/kstdlib.h>
#include <wt/kstdlib/kutoa.h>

char *
    kutoa (char *buf, char *end, unsigned long value, int base, int is_uppercase) {
	if ( base <= 1 ) {
		if ( buf < end )
			*buf = '\0';
		return buf;
	}
	const char *digit_set = is_uppercase ? kdigits_upper : kdigits_lower;
	char	   *rev	      = buf;

	if ( value == 0 ) {
		if ( buf < end ) {
			*rev++ = '0';
		}
		return rev;
	}

	while ( value != 0 && rev < end ) {
		*rev++ = digit_set[(unsigned) (value % (unsigned) base)];
		value /= (unsigned) base;
	}

	// Reverse string
	for ( char *p1 = buf, *p2 = rev - 1; p1 < p2; p1++, p2-- ) {
		char tmp = *p1;
		*p1	 = *p2;
		*p2	 = tmp;
	}

	return rev;
}
