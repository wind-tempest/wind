// SPDX-License-Identifier: LSL-1.4
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#pragma once

extern struct Debug debug;

struct Debug
{
	void (*crit)(const char *, const char *, const char *);
	void (*alert)(const char *, const char *, const char *);
	void (*emerg)(const char *, const char *, const char *);
	void (*warn)(const char *, const char *, const char *);
	void (*err)(const char *, const char *, const char *);
	void (*notice)(const char *, const char *, const char *);
	void (*info)(const char *, const char *, const char *);
	void (*uts)(const char *);               // debug.uts
	int (*printf)(const char *format, ...);  // kdebugf
};
