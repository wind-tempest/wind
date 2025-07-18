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
    puthex (uint64_t value);
extern void
    putdec (uint32_t value);

static volatile uint32_t *framebuffer = NULL;
uint32_t		  fb_width    = 0;
uint32_t		  fb_height   = 0;
uint32_t		  fb_pitch    = 0;
uint8_t			  fb_bpp      = 0;

static uint32_t cursor_x = 0;
static uint32_t cursor_y = 0;

void
    video_clear (uint32_t color)
{
	if ( framebuffer == NULL )
		return;

	for ( uint32_t y = 0; y < fb_height; y++ )
	{
		volatile uint8_t *row_base = (volatile uint8_t *) framebuffer + y * fb_pitch;

		if ( fb_bpp == 16 )
		{
			volatile uint16_t *row = (volatile uint16_t *) row_base;
			uint16_t color16 = (uint16_t) (color & 0xFFFF); // Assume formato RGB565
			for ( uint32_t x = 0; x < fb_width; x++ )
			{
				row[x] = color16;
			}
		}
		else if ( fb_bpp == 24 )
		{
			volatile uint8_t *row = row_base;
			uint8_t		  r   = (color >> 16) & 0xFF;
			uint8_t		  g   = (color >> 8) & 0xFF;
			uint8_t		  b   = (color >> 0) & 0xFF;
			for ( uint32_t x = 0; x < fb_width; x++ )
			{
				row[x * 3 + 0] = b;
				row[x * 3 + 1] = g;
				row[x * 3 + 2] = r;
			}
		}
		else if ( fb_bpp == 32 )
		{
			volatile uint32_t *row = (volatile uint32_t *) row_base;
			for ( uint32_t x = 0; x < fb_width; x++ )
			{
				row[x] = color;
			}
		}
	}

	cursor_x = 0;
	cursor_y = 0;
}

/* Convert 24-bit RGB (0xRRGGBB) to 16-bit RGB565 */
static inline uint16_t
    rgb888_to_rgb565 (uint32_t rgb)
{
	uint8_t r = (rgb >> 16) & 0xFF;
	uint8_t g = (rgb >> 8) & 0xFF;
	uint8_t b = rgb & 0xFF;
	return (uint16_t) ((r >> 3) << 11 | (g >> 2) << 5 | (b >> 3));
}

bool
    is_video_ready (void)
{
	return framebuffer != NULL && fb_width > 0 && fb_height > 0;
}

static bool
    is_hex_char (char c)
{
	return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

uint32_t
    rgb_to_bgr (uint32_t rgb)
{
	uint8_t r = (rgb >> 16) & 0xFF;
	uint8_t g = (rgb >> 8) & 0xFF;
	uint8_t b = rgb & 0xFF;
	return ((uint32_t) b << 16) | ((uint32_t) g << 8) | (uint32_t) r;
}

uint32_t
    hexstr_to_color (const char *hex)
{
	if ( hex[0] == '#' )
		hex++; // Just fucks up the '#'
	if ( hex[0] == '0' && (hex[1] == 'x' || hex[1] == 'X') )
		hex += 2;

	uint32_t value = 0;
	for ( int i = 0; i < 6 && is_hex_char(hex[i]); i++ )
	{
		char c = hex[i];
		value <<= 4;
		if ( c >= '0' && c <= '9' )
			value |= (uint32_t) (c - '0');
		else if ( c >= 'a' && c <= 'f' )
			value |= (uint32_t) (c - 'a' + 10);
		else if ( c >= 'A' && c <= 'F' )
			value |= (uint32_t) (c - 'A' + 10);
	}

	return value;
}

void
    video_init (struct framebuffer_info *fb_info)
{
	if ( fb_info == NULL || fb_info->addr == 0 )
	{
		return;
	}

	framebuffer = (volatile uint32_t *) fb_info->addr;
	fb_width    = fb_info->width;
	fb_height   = fb_info->height;
	fb_pitch    = fb_info->pitch;
	fb_bpp	    = fb_info->bpp;

	if ( fb_width == 0 || fb_height == 0 )
	{
		kputs("video_init: invalid framebuffer dimensions");
		return;
	}

	cursor_x = 0;
	cursor_y = 0;
}

void
    video_put_pixel (uint32_t x, uint32_t y, uint32_t rgb_color)
{
	if ( framebuffer == NULL || x >= fb_width || y >= fb_height )
	{
		return;
	}

	if ( fb_bpp == 16 )
	{
		uint16_t *row = (uint16_t *) ((uint8_t *) framebuffer + y * fb_pitch);
		row[x]	      = rgb888_to_rgb565(rgb_color);
	}
	else
	{
		uint32_t *row = (uint32_t *) ((uint8_t *) framebuffer + y * fb_pitch);
		row[x]	      = rgb_to_bgr(rgb_color);
	}
}

static void
    video_draw_glyph_at (char c, uint32_t x, uint32_t y, uint32_t rgb_color)
{
	if ( framebuffer == NULL || (unsigned int) c >= 256 )
	{
		return;
	}

	const unsigned char *glyph = font[(unsigned int) c];

	for ( uint32_t row = 0; row < FONT_HEIGHT; row++ )
	{
		unsigned char row_data = glyph[row];
		for ( uint32_t col = 0; col < FONT_WIDTH; col++ )
		{
			if ( row_data & (0x80 >> col) )
			{
				video_put_pixel(x + col, y + row, rgb_color);
			}
		}
	}
}

void
    video_putchar (char c)
{
	if ( c == '\b' )
	{
		if ( cursor_x >= FONT_WIDTH )
		{
			cursor_x -= FONT_WIDTH;
		}
		else if ( cursor_y >= FONT_HEIGHT )
		{
			cursor_y -= FONT_HEIGHT;
			cursor_x = fb_width - FONT_WIDTH;
		}
		/* Erase the character at the current position */
		for ( uint32_t row = 0; row < FONT_HEIGHT; row++ )
		{
			for ( uint32_t col = 0; col < FONT_WIDTH; col++ )
			{
				video_put_pixel(cursor_x + col, cursor_y + row, 0x000000);
			}
		}
		return;
	}

	if ( c == '\n' )
	{
		cursor_x = 0;
		cursor_y += FONT_HEIGHT;
	}
	else if ( c == '\r' )
	{
		cursor_x = 0;
	}
	else
	{
		video_draw_glyph_at(c, cursor_x, cursor_y, 0xFFFFFF); // White (RGB)
		cursor_x += FONT_WIDTH;
	}

	if ( cursor_x + FONT_WIDTH > fb_width )
	{
		cursor_x = 0;
		cursor_y += FONT_HEIGHT;
	}

	if ( cursor_y + FONT_HEIGHT > fb_height )
	{
		if ( is_video_ready() )
		{
			video_clear(0x000000);
		}
	}
}

void
    video_draw_circle (int cx, int cy, int radius, uint32_t rgb_color)
{
	for ( int y = -radius; y <= radius; y++ )
	{
		for ( int x = -radius; x <= radius; x++ )
		{
			if ( x * x + y * y <= radius * radius )
			{
				int px = cx + x;
				int py = cy + y;
				if ( px >= 0 && py >= 0 )
				{
					video_put_pixel((uint32_t) px, (uint32_t) py, rgb_color);
				}
			}
		}
	}
}

void
    video_draw_square (int cx, int cy, int size, uint32_t rgb_color)
{
	int half = size / 2;

	for ( int y = cy - half; y < cy + half; y++ )
	{
		for ( int x = cx - half; x < cx + half; x++ )
		{
			if ( x >= 0 && y >= 0 && x < (int) fb_width && y < (int) fb_height )
			{
				video_put_pixel((uint32_t) x, (uint32_t) y, rgb_color);
			}
		}
	}
}

void
    video_puts (const char *s)
{
	while ( *s )
	{
		video_putchar(*s++);
	}
}
