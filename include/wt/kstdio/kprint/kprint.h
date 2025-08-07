// kprint.h

// SPDX-License-Identifier: LSL-1.2-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
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

