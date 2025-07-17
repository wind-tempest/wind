/* katoi.c */

/*
 * Copyright (C) 2025 Wind/Tempest Foundation
 *
 * This file is part of Wind/Tempest.
 *
 * Wind/Tempest is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Wind/Tempest is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
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
