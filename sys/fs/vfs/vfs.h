// SPDX-License-Identifier: LSL-1.4
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#pragma once

#include <lib/kstdio/kstddef.h>

extern struct Vfs vfs;

struct Vfs {
	void (*getcwd)(char *out, ksize_t size);
	void (*normalize)(const char *path, char *out, ksize_t size);
	void (*resolve)(const char *path, char *out, ksize_t size);
	int (*chdir)(const char *path);
};
