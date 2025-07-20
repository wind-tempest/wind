/* video.h */

/*
 * ---------------------------------------------------------------------------
 *  Wind (operating system) / Tempest (kernel)
 *  Copyright (C) 2025 Wind/Tempest Foundation <https://wind.infernointeractive.win>
 * ---------------------------------------------------------------------------
 *  This file is part of the Wind/Tempest project, an open-source operating system
 *  and kernel designed to deliver performance, stability, and freedom to users.
 *
 *  - License:
 *  This software is licensed under the GNU Affero General Public License (AGPL)
 *  version 3 or any later version. You may redistribute and/or modify it under
 *  the terms of the AGPL as published by the Free Software Foundation.
 *
 *  - Warranty Disclaimer:
 *  Wind/Tempest is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more
 *  details.
 *
 *  - Permissions:
 *  > You may modify this file and include your own copyright notices below.
 *  > Forks and contributions are welcome, as long as this license is respected.
 *
 *  - Additional Information:
 *  For more information about the project, visit:
 *  https://wind.infernointeractive.win
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program. If not, see:
 *  https://www.gnu.org/licenses/
 * ---------------------------------------------------------------------------
 */

#pragma once

#include "kstdbool.h"
#include "kstdint.h"

extern kuint32_t fb_width;
extern kuint32_t fb_height;
extern kuint32_t fb_pitch;
extern kuint8_t	 fb_bpp;

/* Framebuffer information structure */
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
    video_init (struct framebuffer_info *fb_info);
kbool
    is_video_ready (void);
kuint32_t
    rgb_to_bgr (kuint32_t rgb);
kuint32_t
    hexstr_to_color (const char *hex);
void
    video_put_pixel (kuint32_t x, kuint32_t y, kuint32_t rgb_color);
void
    video_clear (kuint32_t color);

void
    video_putchar (char c);
void
    video_puts (const char *s);
void
    video_draw_circle (int cx, int cy, int radius, kuint32_t rgb_color);
void
    video_draw_square (int cx, int cy, int size, kuint32_t rgb_color);
