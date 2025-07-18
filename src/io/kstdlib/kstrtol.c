/* kstrtol.c */

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

#include "kctype.h"
#include "kerrno.h"
#include "klimits.h"
#include "kstrtol.h"

long
    kstrtol (const char *nptr, char **endptr, int base)
{
	const char *s	= nptr;
	long	    acc = 0;
	int	    c;
	int	    neg = 0;
	long	    cutoff;
	int	    cutlim;

	/* skip whitespace */
	while ( kisspace((unsigned char) *s) )
		s++;

	/* sign */
	if ( *s == '-' )
	{
		neg = 1;
		s++;
	}
	else if ( *s == '+' )
	{
		s++;
	}

	/* detect base if 0 */
	if ( base == 0 )
	{
		if ( *s == '0' )
		{
			if ( s[1] == 'x' || s[1] == 'X' )
			{
				base = 16;
				s += 2;
			}
			else
			{
				base = 8;
				s++;
			}
		}
		else
		{
			base = 10;
		}
	}
	else if ( base == 16 )
	{
		if ( s[0] == '0' && (s[1] == 'x' || s[1] == 'X') )
		{
			s += 2;
		}
	}

	/* compute cutoff for overflow */
	cutoff = neg ? LONG_MIN : LONG_MAX;
	cutlim = (int) (cutoff % base);
	cutoff /= base;
	if ( cutlim < 0 )
	{
		cutlim += base;
		cutoff += 1;
	}

	/* convert digits */
	for ( ;; s++ )
	{
		c = (unsigned char) *s;
		if ( kisdigit(c) )
			c -= '0';
		else if ( kisalpha(c) )
			c = ktoupper(c) - 'A' + 10;
		else
			break;
		if ( c >= base )
			break;

		/* check overflow */
		if ( neg )
		{
			if ( acc < cutoff || (acc == cutoff && c > cutlim) )
			{
				acc   = LONG_MIN;
				errno = ERANGE;
				neg   = 0; /* to avoid negating again */
			}
			else
			{
				acc *= base;
				acc -= c;
			}
		}
		else
		{
			if ( acc > cutoff || (acc == cutoff && c > cutlim) )
			{
				acc   = LONG_MAX;
				errno = ERANGE;
				neg   = 0; /* to avoid negating again */
			}
			else
			{
				acc *= base;
				acc += c;
			}
		}
	}

	if ( endptr )
		*endptr = (char *) (acc == 0 && s == nptr ? nptr : s);

	return acc;
}
