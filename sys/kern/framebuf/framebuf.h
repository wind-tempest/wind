// SPDX-License-Identifier: LSL-1.4
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#pragma once

#include "drivers/video/video.h"

#include <lib/kstdio/kstdint.h>

extern struct framebuffer_info fb_info;

void
    map_framebuffer_address (kuint64_t phys_addr);
