/* katoi.c */

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

#include "katoi.h"
#include "kctype.h"
#include "kerrno.h"
#include "klimits.h"

int
    katoi (const char *s)
{
	long result = 0;
	int  sign   = 1;

	errno = 0;

	while ( kisspace(*s) )
		s++;

	if ( *s == '+' || *s == '-' )
	{
		if ( *s == '-' )
			sign = -1;
		s++;
	}

	if ( !kisdigit(*s) )
	{
		errno = EINVAL;
		return 0;
	}

	while ( kisdigit(*s) )
	{
		int digit = *s - '0';

		if ( result > (LONG_MAX - digit) / 10 )
		{
			errno = ERANGE;
			return sign == 1 ? INT_MAX : INT_MIN;
		}

		result = result * 10 + digit;
		s++;
	}

	result *= sign;

	if ( result < INT_MIN || result > INT_MAX )
	{
		errno = ERANGE;
		return result > 0 ? INT_MAX : INT_MIN;
	}

	return (int) result;
}
