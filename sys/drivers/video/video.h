// SPDX-License-Identifier: LSL-1.4
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#pragma once

#include <lib/kstdio/kstdbool.h>
#include <lib/kstdio/kstdint.h>

extern struct video video;

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

struct video {
	void (*init)(struct framebuffer_info *fb_info);
	kbool (*is_ready)(void);
	kuint16_t (*rgb888_to_rgb565)(kuint32_t rgb);
	kuint32_t (*rgb_to_bgr)(kuint32_t rgb);
	kuint32_t (*hex_to_color)(const char *hex);
	void (*put_pixel)(kuint32_t x, kuint32_t y, kuint32_t rgb_color);
	void (*clear)(kuint32_t color);
	void (*put_char)(char c);
	void (*puts)(const char *s);
	void (*draw_circle)(int cx, int cy, int radius, kuint32_t rgb_color);
	void (*draw_square)(int cx, int cy, int size, kuint32_t rgb_color);
};
