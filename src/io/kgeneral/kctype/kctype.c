/* kctype.c */

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

int
    kisupper (int c) {
	return c >= 'A' && c <= 'Z';
}

int
    kislower (int c) {
	return c >= 'a' && c <= 'z';
}

int
    kisalpha (int c) {
	return kisupper(c) || kislower(c);
}

int
    kisdigit (int c) {
	return c >= '0' && c <= '9';
}

int
    kisxdigit (int c) {
	return kisdigit(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
}

int
    kisalnum (int c) {
	return kisalpha(c) || kisdigit(c);
}

int
    kisspace (int c) {
	return c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v';
}

int
    kisblank (int c) {
	return c == ' ' || c == '\t';
}

int
    kiscntrl (int c) {
	return (c >= 0 && c < 0x20) || c == 0x7F;
}

int
    kisgraph (int c) {
	return c > 0x20 && c < 0x7F;
}

int
    kisprint (int c) {
	return c >= 0x20 && c < 0x7F;
}

int
    kispunct (int c) {
	return kisprint(c) && !kisalnum(c) && c != ' ';
}

int
    ktolower (int c) {
	return kisupper(c) ? (c - 'A' + 'a') : c;
}

int
    ktoupper (int c) {
	return kislower(c) ? (c - 'a' + 'A') : c;
}
