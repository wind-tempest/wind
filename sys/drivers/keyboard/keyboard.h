// SPDX-License-Identifier: LSL-2.0
/*
 * -- BEGIN LICENSE HEADER --
 * The Wind/Tempest Project
 *
 * File:        sys/drivers/keyboard/keyboard.h
 * Author(s):   Russian95 <russian95@tempestfoundation.org>
 *              (https://github.com/Russian95CrE)
 * Maintainer:  Tempest Foundation <development@tempestfoundation.org>
 * Link:        https://wtsrc.tempestfoundation.org
 *
 * Copyright (C) 2025 Tempest Foundation
 * Licensed under the Liberty Software License, Version 2.0
 * -- END OF LICENSE HEADER --
 */
#pragma once

#include <arch/amd64/idt.h>

extern struct Keyboard keyboard;

int
    getchar (void);

struct Keyboard {
	void (*init)(void);
	void (*handle)(registers_t *regs);
	unsigned char (*adjust_case)(unsigned char c);
};
