// kctype.c

// SPDX-License-Identifier: LSL-1.2-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include <tempest/kgeneral/kctype/kctype.h>

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
