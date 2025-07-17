/* kctype.c */

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

#include "kctype.h"

int
    kisupper (int c)
{
	return c >= 'A' && c <= 'Z';
}

int
    kislower (int c)
{
	return c >= 'a' && c <= 'z';
}

int
    kisalpha (int c)
{
	return kisupper(c) || kislower(c);
}

int
    kisdigit (int c)
{
	return c >= '0' && c <= '9';
}

int
    kisxdigit (int c)
{
	return kisdigit(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
}

int
    kisalnum (int c)
{
	return kisalpha(c) || kisdigit(c);
}

int
    kisspace (int c)
{
	return c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v';
}

int
    kisblank (int c)
{
	return c == ' ' || c == '\t';
}

int
    kiscntrl (int c)
{
	return (c >= 0 && c < 0x20) || c == 0x7F;
}

int
    kisgraph (int c)
{
	return c > 0x20 && c < 0x7F;
}

int
    kisprint (int c)
{
	return c >= 0x20 && c < 0x7F;
}

int
    kispunct (int c)
{
	return kisprint(c) && !kisalnum(c) && c != ' ';
}

int
    ktolower (int c)
{
	return kisupper(c) ? (c - 'A' + 'a') : c;
}

int
    ktoupper (int c)
{
	return kislower(c) ? (c - 'a' + 'A') : c;
}
