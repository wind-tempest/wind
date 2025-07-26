// serial.h

// SPDX-License-Identifier: LSL-1.0
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.infernointeractive.win>
 *
 * This file is part of the Wind Operating System and Tempest Kernel.
 * It is licensed under the Liberty Software License 1.0 (LSL-1.0).
 * For more information, see <https://wind.infernointeractive.win>.
 */

#pragma once

#include "kstdbool.h"

extern kbool is_serial_available;

void
    serial_init (void);
void
    serial_write_int (int i);
void
    serial_write (char c);
void
    serial_writes (const char *s);
kbool
    serial_available (void);
