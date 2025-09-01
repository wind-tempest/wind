// SPDX-License-Identifier: LSL-1.4
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#pragma once

#include <lib/kstdio/kstdbool.h>

extern struct Serial serial;

struct Serial {
	void (*init)(void);
	void (*write)(char a);
	void (*writes)(const char *s);
	void (*write_int)(int i);
	kbool (*is_available)(void);
};
