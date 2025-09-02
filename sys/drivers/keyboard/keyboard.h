// SPDX-License-Identifier: LSL-1.4
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#pragma once

#include "arch/amd64/idt.h"

extern struct Keyboard keyboard;

int
    getchar (void);

struct Keyboard {
	void (*init)(void);
	void (*handle)(registers_t *regs);
	unsigned char (*adjust_case)(unsigned char c);
};
