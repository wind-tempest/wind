// cpu.h

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.tempestfoundation.org>
 *
 * This file is part of the Wind (Operating System) and Tempest (The Kernel).
 * It is licensed under the GNU General Public License version 3.0  (GPLv3).
 */

#pragma once

#define CPU_BRAND_STRING_LEN 48
extern char cpu_brand_string[CPU_BRAND_STRING_LEN + 1];
void
    kcpu_init_brand (void);
