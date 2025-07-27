// font.h

// SPDX-License-Identifier: LSL-1.2
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.infernointeractive.win>
 *
 * This file is part of the Wind Operating System and Tempest Kernel.
 * It is licensed under the Liberty Software License 1.2 (LSL-1.2).
 * For more information, see <https://wind.infernointeractive.win>.
 */

#pragma once

#include <wt/kstdint.h>

// A simple 8x8 font for rendering text to the framebuffer.
// Each character is 8 pixels wide and 8 pixels tall.
extern const kuint32_t FONT_WIDTH;
extern const kuint32_t FONT_HEIGHT;

extern const unsigned char font[128][8];
