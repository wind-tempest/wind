// kstrnlen.h

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#pragma once

#include <wt/kstdio/kstddef.h>

ksize_t
    kstrnlen (const char *str, ksize_t maxlen);
