// serial.h

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.infernointeractive.win>
 *
 * This file is part of the Wind Operating System and Tempest Kernel.
 * It is licensed under the GNU General Public License version 3.0  (GPLv3).
 */

#pragma once

#include <wt/kstdio/kstdbool.h>

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
