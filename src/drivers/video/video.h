// video.h

// SPDX-License-Identifier: LSL-1.1
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.infernointeractive.win>
 *
 * This file is part of the Wind Operating System and Tempest Kernel.
 * It is licensed under the Liberty Software License 1.1 (LSL-1.1).
 * For more information, see <https://wind.infernointeractive.win>.
 */

// SPDX-License-Identifier: LSL-1.0

#pragma once

#include "kstdbool.h"
#include "kstdint.h"

// Framebuffer information structure
struct framebuffer_info {
	kuint64_t addr;
	kuint32_t pitch;
	kuint32_t width;
	kuint32_t height;
	kuint8_t  bpp;
	kuint8_t  type;
	kuint8_t  red_mask_size;
	kuint8_t  red_mask_shift;
	kuint8_t  green_mask_size;
	kuint8_t  green_mask_shift;
	kuint8_t  blue_mask_size;
	kuint8_t  blue_mask_shift;
};

void
    kvideo_init (struct framebuffer_info *fb_info);
kbool
    kis_video_ready (void);
kuint32_t
    krgb_to_bgr (kuint32_t rgb);
kuint32_t
    khexstr_to_color (const char *hex);
void
    kvideo_put_pixel (kuint32_t x, kuint32_t y, kuint32_t rgb_color);
void
    kvideo_clear (kuint32_t color);

void
    kvideo_putchar (char c);
void
    kvideo_puts (const char *s);
void
    kvideo_draw_circle (int cx, int cy, int radius, kuint32_t rgb_color);
void
    kvideo_draw_square (int cx, int cy, int size, kuint32_t rgb_color);
