// video.h

// SPDX-License-Identifier: LSL-1.2-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#pragma once

#include <tempest/klibc/kstdio/kstdbool.h>
#include <tempest/klibc/kstdio/kstdint.h>

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
