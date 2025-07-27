// kstrtol.c

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

#include <wt/kgeneral/kctype/kctype.h>
#include <wt/kgeneral/kerrno/kerrno.h>
#include <wt/klimits.h>
#include <wt/kstdlib/kstdlib.h>
#include <wt/kstdlib/kstrtol.h>

long
    kstrtol (const char *nptr, char **endptr, int base) {
	const char *s	= nptr;
	long	    acc = 0;
	int	    c;
	int	    neg = 0;
	long	    cutoff;
	int	    cutlim;

	// skip whitespace
	while ( kisspace((unsigned char) *s) )
		s++;

	// sign
	if ( *s == '-' ) {
		neg = 1;
		s++;
	} else if ( *s == '+' ) {
		s++;
	}

	// detect base if 0
	if ( base == 0 ) {
		if ( *s == '0' ) {
			if ( s[1] == 'x' || s[1] == 'X' ) {
				base = 16;
				s += 2;
			} else {
				base = 8;
				s++;
			}
		} else {
			base = 10;
		}
	} else if ( base == 16 ) {
		if ( s[0] == '0' && (s[1] == 'x' || s[1] == 'X') ) {
			s += 2;
		}
	}

	// compute cutoff for overflow
	cutoff = neg ? KLONG_MIN : KLONG_MAX;
	cutlim = (int) (cutoff % base);
	cutoff /= base;
	if ( cutlim < 0 ) {
		cutlim += base;
		cutoff += 1;
	}

	// convert digits
	for ( ;; s++ ) {
		c = (unsigned char) *s;
		if ( kisdigit(c) )
			c -= '0';
		else if ( kisalpha(c) )
			c = ktoupper(c) - 'A' + 10;
		else
			break;
		if ( c >= base )
			break;

		// check overflow
		if ( neg ) {
			if ( acc < cutoff || (acc == cutoff && c > cutlim) ) {
				acc    = KLONG_MIN;
				kerrno = KERANGE;
				neg    = 0; // to avoid negating again
			} else {
				acc *= base;
				acc -= c;
			}
		} else {
			if ( acc > cutoff || (acc == cutoff && c > cutlim) ) {
				acc    = KLONG_MAX;
				kerrno = KERANGE;
				neg    = 0; // to avoid negating again
			} else {
				acc *= base;
				acc += c;
			}
		}
	}

	if ( endptr )
		*endptr = (char *) (acc == 0 && s == nptr ? nptr : s);

	return acc;
}
