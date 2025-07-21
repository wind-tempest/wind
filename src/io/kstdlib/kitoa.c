/* kitoa.c */

/*
 * ---------------------------------------------------------------------------
 *  Wind (operating system) / Tempest (kernel)
 *  Copyright (C) 2025 Wind/Tempest Foundation <https://wind.infernointeractive.win>
 * ---------------------------------------------------------------------------
 *  This file is part of the Wind/Tempest project, an open-source operating system
 *  and kernel designed to deliver performance, stability, and freedom to users.
 *
 *  - License:
 *  This software is licensed under the GNU Affero General Public License (AGPL)
 *  version 3 or any later version. You may redistribute and/or modify it under
 *  the terms of the AGPL as published by the Free Software Foundation.
 *
 *  - Warranty Disclaimer:
 *  Wind/Tempest is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more
 *  details.
 *
 *  - Permissions:
 *  > You may modify this file and include your own copyright notices below.
 *  > Forks and contributions are welcome, as long as this license is respected.
 *
 *  - Additional Information:
 *  For more information about the project, visit:
 *  https://wind.infernointeractive.win
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program. If not, see:
 *  https://www.gnu.org/licenses/
 * ---------------------------------------------------------------------------
 */

#include "kcharset.h"
#include "kitoa.h"

char *
    kitoa (char *buf, char *end, long value, int base, int is_uppercase) {
	if ( base <= 1 ) {
		if ( buf < end )
			*buf = '\0';
		return buf;
	}
	const char *digit_set = is_uppercase ? kdigits_upper : kdigits_lower;
	char	   *rev	      = buf;
	int	    negative  = 0;

	/* Ensure we have at least 2 bytes for null terminator */
	if ( buf >= end ) {
		if ( buf < end ) {
			*buf = '\0';
		}
		return buf;
	}

	if ( value == 0 ) {
		*rev++ = '0';
		*rev   = '\0';
		return rev;
	}

	if ( value < 0 ) {
		negative = 1;
		value	 = -value;
	}

	while ( value != 0 && rev < end - 1 ) {
		*rev++ = digit_set[(unsigned) (value % (unsigned) base)];
		value /= (unsigned) base;
	}

	if ( negative && rev < end - 1 ) {
		*rev++ = '-';
	}

	/* Reverse string */
	for ( char *p1 = buf, *p2 = rev - 1; p1 < p2; p1++, p2-- ) {
		char tmp = *p1;
		*p1	 = *p2;
		*p2	 = tmp;
	}

	return rev;
}
