// SPDX-License-Identifier: LSL-1.3
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#pragma once

#include <tempest/klibc/kstdio/kstdbool.h>

void
    serial_init (void);
void
    serial_write_int (int i);
void
    serial_write (char c);
void
    serial_writes (const char *s);
kbool
    is_serial_available (void);

