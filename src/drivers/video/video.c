/* video.c */

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

#include "font/font.h"
#include "kstdbool.h"
#include "kstddef.h"
#include "video.h"

/* Forward declarations */
extern void
    kputs (const char *s);
extern void
    puthex (kuint64_t value);
extern void
    putdec (kuint32_t value);

static volatile kuint32_t *framebuffer = KNULL;
kuint32_t		   fb_width    = 0;
kuint32_t		   fb_height   = 0;
kuint32_t		   fb_pitch    = 0;
kuint8_t		   fb_bpp      = 0;

static kuint32_t cursor_x = 0;
static kuint32_t cursor_y = 0;

void
    video_clear (kuint32_t color) {
	if ( framebuffer == KNULL )
		return;

	for ( kuint32_t y = 0; y < fb_height; y++ ) {
		volatile kuint8_t *row_base = (volatile kuint8_t *) framebuffer + y * fb_pitch;

		if ( fb_bpp == 16 ) {
			volatile kuint16_t *row = (volatile kuint16_t *) row_base;
			kuint16_t color16 = (kuint16_t) (color & 0xFFFF); // Assume formato RGB565
			for ( kuint32_t x = 0; x < fb_width; x++ ) {
				row[x] = color16;
			}
		} else if ( fb_bpp == 24 ) {
			volatile kuint8_t *row = row_base;
			kuint8_t	   r   = (color >> 16) & 0xFF;
			kuint8_t	   g   = (color >> 8) & 0xFF;
			kuint8_t	   b   = (color >> 0) & 0xFF;
			for ( kuint32_t x = 0; x < fb_width; x++ ) {
				row[x * 3 + 0] = b;
				row[x * 3 + 1] = g;
				row[x * 3 + 2] = r;
			}
		} else if ( fb_bpp == 32 ) {
			volatile kuint32_t *row = (volatile kuint32_t *) row_base;
			for ( kuint32_t x = 0; x < fb_width; x++ ) {
				row[x] = color;
			}
		}
	}

	cursor_x = 0;
	cursor_y = 0;
}

/* Convert 24-bit RGB (0xRRGGBB) to 16-bit RGB565 */
static inline kuint16_t
    rgb888_to_rgb565 (kuint32_t rgb) {
	kuint8_t r = (rgb >> 16) & 0xFF;
	kuint8_t g = (rgb >> 8) & 0xFF;
	kuint8_t b = rgb & 0xFF;
	return (kuint16_t) ((r >> 3) << 11 | (g >> 2) << 5 | (b >> 3));
}

kbool
    is_video_ready (void) {
	return framebuffer != KNULL && fb_width > 0 && fb_height > 0;
}

static kbool
    is_hex_char (char c) {
	return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

kuint32_t
    rgb_to_bgr (kuint32_t rgb) {
	kuint8_t r = (rgb >> 16) & 0xFF;
	kuint8_t g = (rgb >> 8) & 0xFF;
	kuint8_t b = rgb & 0xFF;
	return ((kuint32_t) b << 16) | ((kuint32_t) g << 8) | (kuint32_t) r;
}

kuint32_t
    hexstr_to_color (const char *hex) {
	if ( hex[0] == '#' )
		hex++; // Just fucks up the '#'
	if ( hex[0] == '0' && (hex[1] == 'x' || hex[1] == 'X') )
		hex += 2;

	kuint32_t value = 0;
	for ( int i = 0; i < 6 && is_hex_char(hex[i]); i++ ) {
		char c = hex[i];
		value <<= 4;
		if ( c >= '0' && c <= '9' )
			value |= (kuint32_t) (c - '0');
		else if ( c >= 'a' && c <= 'f' )
			value |= (kuint32_t) (c - 'a' + 10);
		else if ( c >= 'A' && c <= 'F' )
			value |= (kuint32_t) (c - 'A' + 10);
	}

	return value;
}

void
    video_init (struct framebuffer_info *fb_info) {
	if ( fb_info == KNULL || fb_info->addr == 0 ) {
		return;
	}

	framebuffer = (volatile kuint32_t *) fb_info->addr;
	fb_width    = fb_info->width;
	fb_height   = fb_info->height;
	fb_pitch    = fb_info->pitch;
	fb_bpp	    = fb_info->bpp;

	if ( fb_width == 0 || fb_height == 0 ) {
		kputs("video_init: invalid framebuffer dimensions");
		return;
	}

	cursor_x = 0;
	cursor_y = 0;
}

void
    video_put_pixel (kuint32_t x, kuint32_t y, kuint32_t rgb_color) {
	if ( framebuffer == KNULL || x >= fb_width || y >= fb_height ) {
		return;
	}

	if ( fb_bpp == 16 ) {
		kuint16_t *row = (kuint16_t *) ((kuint8_t *) framebuffer + y * fb_pitch);
		row[x]	       = rgb888_to_rgb565(rgb_color);
	} else {
		kuint32_t *row = (kuint32_t *) ((kuint8_t *) framebuffer + y * fb_pitch);
		row[x]	       = rgb_to_bgr(rgb_color);
	}
}

static void
    video_draw_glyph_at (char c, kuint32_t x, kuint32_t y, kuint32_t rgb_color) {
	if ( framebuffer == KNULL || (unsigned int) c >= 256 ) {
		return;
	}

	const unsigned char *glyph = font[(unsigned int) c];

	for ( kuint32_t row = 0; row < FONT_HEIGHT; row++ ) {
		unsigned char row_data = glyph[row];
		for ( kuint32_t col = 0; col < FONT_WIDTH; col++ ) {
			if ( row_data & (0x80 >> col) ) {
				video_put_pixel(x + col, y + row, rgb_color);
			}
		}
	}
}

void
    video_putchar (char c) {
	if ( c == '\b' ) {
		if ( cursor_x >= FONT_WIDTH ) {
			cursor_x -= FONT_WIDTH;
		} else if ( cursor_y >= FONT_HEIGHT ) {
			cursor_y -= FONT_HEIGHT;
			cursor_x = fb_width - FONT_WIDTH;
		}
		/* Erase the character at the current position */
		for ( kuint32_t row = 0; row < FONT_HEIGHT; row++ ) {
			for ( kuint32_t col = 0; col < FONT_WIDTH; col++ ) {
				video_put_pixel(cursor_x + col, cursor_y + row, 0x000000);
			}
		}
		return;
	}

	if ( c == '\n' ) {
		cursor_x = 0;
		cursor_y += FONT_HEIGHT;
	} else if ( c == '\r' ) {
		cursor_x = 0;
	} else {
		video_draw_glyph_at(c, cursor_x, cursor_y, 0xFFFFFF);
		cursor_x += FONT_WIDTH;
	}

	if ( cursor_x + FONT_WIDTH > fb_width ) {
		cursor_x = 0;
		cursor_y += FONT_HEIGHT;
	}

	if ( cursor_y + FONT_HEIGHT > fb_height ) {
		if ( is_video_ready() ) {
			video_clear(0x000000);
		}
	}
}

void
    video_draw_circle (int cx, int cy, int radius, kuint32_t rgb_color) {
	for ( int y = -radius; y <= radius; y++ ) {
		for ( int x = -radius; x <= radius; x++ ) {
			if ( x * x + y * y <= radius * radius ) {
				int px = cx + x;
				int py = cy + y;
				if ( px >= 0 && py >= 0 ) {
					video_put_pixel((kuint32_t) px, (kuint32_t) py, rgb_color);
				}
			}
		}
	}
}

void
    video_draw_square (int cx, int cy, int size, kuint32_t rgb_color) {
	int half = size / 2;

	for ( int y = cy - half; y < cy + half; y++ ) {
		for ( int x = cx - half; x < cx + half; x++ ) {
			if ( x >= 0 && y >= 0 && x < (int) fb_width && y < (int) fb_height ) {
				video_put_pixel((kuint32_t) x, (kuint32_t) y, rgb_color);
			}
		}
	}
}

void
    video_puts (const char *s) {
	while ( *s ) {
		video_putchar(*s++);
	}
}
