// SPDX-License-Identifier: LSL-1.3
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <tempest/klibc/kgeneral/kctype/kctype.h>
#include <tempest/klibc/kgeneral/kerrno/kerrno.h>
#include <tempest/klibc/klimits.h>
#include <tempest/klibc/kstdlib/katoi.h>
#include <tempest/klibc/kstdlib/kstdlib.h>

int
    katoi (const char *s) {
	long result = 0;
	int  sign   = 1;

	kerrno = 0;

	while (kisspace(*s))
		s++;

	if (*s == '+' || *s == '-') {
		if (*s == '-')
			sign = -1;
		s++;
	}

	if (!kisdigit(*s)) {
		kerrno = KEINVAL;
		return 0;
	}

	while (kisdigit(*s)) {
		int digit = *s - '0';

		if (result > (KLONG_MAX - digit) / 10) {
			kerrno = KERANGE;
			return sign == 1 ? KINT_MAX : KINT_MIN;
		}

		result = result * 10 + digit;
		s++;
	}

	result *= sign;

	if (result < KINT_MIN || result > KINT_MAX) {
		kerrno = KERANGE;
		return result > 0 ? KINT_MAX : KINT_MIN;
	}

	return (int) result;
}

