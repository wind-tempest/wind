// kutoa.c

// SPDX-License-Identifier: LSL-1.2
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.infernointeractive.win>
 *
 * This file is part of the Wind Operating System and Tempest Kernel.
 * It is licensed under the Liberty Software License 1.2 (LSL-1.2).
 * For more information, see <https://wind.infernointeractive.win>.
 */

#include <wt/kstdlib/kutoa.h>

#include <wt/kgeneral/kcharset/kcharset.h>
#include <wt/kstdlib/kstdlib.h>

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
