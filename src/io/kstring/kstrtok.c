/* kstrtok.c */

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

#include "kstddef.h"
#include "kstrchr.h"
#include "kstrtok.h"

static char *strtok_save = NULL;

char *
    kstrtok (char *str, const char *delim)
{
	if ( str != NULL )
	{
		strtok_save = str;
	}
	else if ( strtok_save == NULL )
	{
		return NULL;
	}

	/* Skip leading delimiters */
	while ( *strtok_save && kstrchr(delim, *strtok_save) )
	{
		strtok_save++;
	}

	if ( *strtok_save == '\0' )
	{
		strtok_save = NULL;
		return NULL;
	}

	char *token_start = strtok_save;

	/* Find end of token */
	while ( *strtok_save && !kstrchr(delim, *strtok_save) )
	{
		strtok_save++;
	}

	if ( *strtok_save != '\0' )
	{
		*strtok_save = '\0';
		strtok_save++;
	}
	else
	{
		strtok_save = NULL;
	}

	return token_start;
}
