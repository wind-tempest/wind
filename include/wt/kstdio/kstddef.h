// kstddef.h

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

#include <wt/kstdio/kstdint.h>

#ifndef KNULL
#	define KNULL ((void *) 0)
#endif

typedef kuint64_t ksize_t;
typedef long	  kptrdiff_t;
