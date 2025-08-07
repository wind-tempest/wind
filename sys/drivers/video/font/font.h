// font.h

// SPDX-License-Identifier: LSL-1.2-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#pragma once

#include <wt/kstdio/kstdint.h>

// A simple 8x8 font for rendering text to the framebuffer.
// Each character is 8 pixels wide and 8 pixels tall.
extern const kuint32_t FONT_WIDTH;
extern const kuint32_t FONT_HEIGHT;

extern const unsigned char font[128][8];

