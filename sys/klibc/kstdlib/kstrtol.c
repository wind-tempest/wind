// kstrtol.c

// SPDX-License-Identifier: LSL-1.2-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <tempest/klibc/kgeneral/kctype/kctype.h>
#include <tempest/klibc/kgeneral/kerrno/kerrno.h>
#include <tempest/klibc/klimits.h>
#include <tempest/klibc/kstdlib/kstdlib.h>
#include <tempest/klibc/kstdlib/kstrtol.h>

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
				neg    = 0;  // to avoid negating again
			} else {
				acc *= base;
				acc -= c;
			}
		} else {
			if ( acc > cutoff || (acc == cutoff && c > cutlim) ) {
				acc    = KLONG_MAX;
				kerrno = KERANGE;
				neg    = 0;  // to avoid negating again
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
