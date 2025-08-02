// kprint.h

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

#include <wt/kstdarg.h>
#include <wt/kstdio/kstddef.h>
#include <wt/kstdio/kstdint.h>

void
    kputhex (kuint64_t n);
void
    kputdec (kuint32_t n);
void
    kputs (const char *s);
int
    kvsnprintf (char *buffer, ksize_t size, const char *format, va_list args);
int
    ksnprintf (char *buffer, ksize_t size, const char *format, ...);
int
    kprintf (const char *format, ...);
void
    kputchar (int c);
