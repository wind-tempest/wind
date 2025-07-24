// video.c

/*
 * ============================================================================
 *  Wind Operating System / Tempest Kernel
 *  Copyright (C) 2025 Wind/Tempest Foundation <https://wind.infernointeractive.win>
 * ============================================================================
 *
 *  Overview:
 *  This source file is part of the Wind OS and Tempest Kernel project, a fully
 *  open-source operating system kernel developed to provide a robust, scalable,
 *  and efficient platform optimized for modern hardware and user-centric freedom.
 *
 *  Project Scope:
 *  Wind/Tempest targets a modular architecture supporting symmetric multiprocessing,
 *  preemptive multitasking, fine-grained memory management, and extensible driver
 *  frameworks. The system emphasizes low latency, high throughput, and
 *  predictable behavior for both user-space applications and kernel subsystems.
 *
 *  Licensing Terms:
 *  The Wind/Tempest source code is licensed under the GNU Affero General Public
 *  License (AGPL) version 3 or any later version, published by the Free Software Foundation.
 *  This license guarantees that all derivative works remain open and freely
 *  available, particularly addressing network use cases to enforce sharing.
 *
 *  You are authorized to copy, modify, distribute, and use this software,
 *  provided that all modifications and derivatives comply with the AGPL terms.
 *  Inclusion of proper attribution and licensing notices is mandatory in redistributions.
 *
 *  Warranty and Liability Disclaimer:
 *  This software is provided "as-is", without any express or implied warranty,
 *  including but not limited to implied warranties of merchantability, fitness for
 *  a particular purpose, or non-infringement of third-party rights.
 *  Under no circumstances shall the authors, contributors, or copyright holders be liable
 *  for any damages arising directly or indirectly from the use or inability to use
 *  this software, including data loss, system failures, or other incidental damages.
 *
 *  Contribution Guidelines:
 *  Contributions to the Wind/Tempest project are welcome and encouraged.
 *  Contributors must agree to license their contributions under the same AGPL terms.
 *  When submitting patches, please ensure they adhere to the project's coding standards,
 *  include proper documentation, and maintain backward compatibility where possible.
 *
 *  Security and Stability:
 *  The kernel maintains rigorous coding practices to minimize security vulnerabilities.
 *  Regular code audits, testing, and community reviews are integral to the project's
 *  ongoing stability and security posture.
 *  Users and developers should report issues promptly via official channels.
 *
 *  Documentation and Support:
 *  Comprehensive documentation including API references, architecture overviews,
 *  and development guides are available on the official website:
 *    https://wind.infernointeractive.win
 *  Community support can be found through mailing lists, forums, and issue trackers
 *  hosted on the project's main repository.
 *
 *  Additional Notes:
 *  This kernel is designed to be hardware agnostic, supporting a wide range of
 *  architectures and devices. Modularity in subsystems allows customization
 *  for embedded, desktop, and server deployments.
 *
 *  By using or modifying this code, you agree to uphold the principles of free and
 *  open-source software, contributing back improvements to foster collective progress.
 *
 *  A copy of the full GNU Affero General Public License should accompany this software.
 *  If you have not received it, it can be obtained at:
 *    https://www.gnu.org/licenses/agpl-3.0.html
 *
 * ============================================================================
 */

#include "video.h"

#include "core/entry/kernel.h"
#include "font/font.h"
#include "kstdbool.h"
#include "kstddef.h"

// Forward declarations
extern void
    kputs (const char *s);
extern void
    puthex (kuint64_t value);
extern void
    putdec (kuint32_t value);

static volatile kuint32_t *framebuffer = KNULL;

static kuint32_t cursor_x = 0;
static kuint32_t cursor_y = 0;

void
    kvideo_clear (kuint32_t color) {
	if ( framebuffer == KNULL )
		return;

	for ( kuint32_t y = 0; y < fb_info.height; y++ ) {
		volatile kuint8_t *row_base = (volatile kuint8_t *) framebuffer + y * fb_info.pitch;

		if ( fb_info.bpp == 16 ) {
			volatile kuint16_t *row = (volatile kuint16_t *) row_base;
			kuint16_t color16 = (kuint16_t) (color & 0xFFFF); // Assume formato RGB565
			for ( kuint32_t x = 0; x < fb_info.width; x++ ) {
				row[x] = color16;
			}
		} else if ( fb_info.bpp == 24 ) {
			volatile kuint8_t *row = row_base;
			kuint8_t	   r   = (color >> 16) & 0xFF;
			kuint8_t	   g   = (color >> 8) & 0xFF;
			kuint8_t	   b   = (color >> 0) & 0xFF;
			for ( kuint32_t x = 0; x < fb_info.width; x++ ) {
				row[x * 3 + 0] = b;
				row[x * 3 + 1] = g;
				row[x * 3 + 2] = r;
			}
		} else if ( fb_info.bpp == 32 ) {
			volatile kuint32_t *row = (volatile kuint32_t *) row_base;
			for ( kuint32_t x = 0; x < fb_info.width; x++ ) {
				row[x] = color;
			}
		}
	}

	cursor_x = 0;
	cursor_y = 0;
}

// Convert 24-bit RGB (0xRRGGBB) to 16-bit RGB565
static inline kuint16_t
    krgb888_to_rgb565 (kuint32_t rgb) {
	kuint8_t r = (rgb >> 16) & 0xFF;
	kuint8_t g = (rgb >> 8) & 0xFF;
	kuint8_t b = rgb & 0xFF;
	return (kuint16_t) ((r >> 3) << 11 | (g >> 2) << 5 | (b >> 3));
}

kbool
    kis_video_ready (void) {
	return fb_info.addr != 0 && fb_info.width > 0 && fb_info.height > 0;
}

static kbool
    kis_hex_char (char c) {
	return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

kuint32_t
    krgb_to_bgr (kuint32_t rgb) {
	kuint8_t r = (rgb >> 16) & 0xFF;
	kuint8_t g = (rgb >> 8) & 0xFF;
	kuint8_t b = rgb & 0xFF;
	return ((kuint32_t) b << 16) | ((kuint32_t) g << 8) | (kuint32_t) r;
}

kuint32_t
    khexstr_to_color (const char *hex) {
	if ( hex[0] == '#' )
		hex++; // Just fucks up the '#'
	if ( hex[0] == '0' && (hex[1] == 'x' || hex[1] == 'X') )
		hex += 2;

	kuint32_t value = 0;
	for ( int i = 0; i < 6 && kis_hex_char(hex[i]); i++ ) {
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
    kvideo_init (struct framebuffer_info *fb) {
	if ( !kis_video_ready() ) {
		return;
	}

	framebuffer = (volatile kuint32_t *) fb->addr;

	if ( fb_info.width == 0 || fb_info.height == 0 ) {
		kputs("kvideo_init: invalid framebuffer dimensions");
		return;
	}

	cursor_x = 0;
	cursor_y = 0;
}

void
    kvideo_put_pixel (kuint32_t x, kuint32_t y, kuint32_t rgb_color) {
	if ( !kis_video_ready() ) {
		return;
	}

	if ( fb_info.bpp == 16 ) {
		kuint16_t *row = (kuint16_t *) ((kuint8_t *) framebuffer + y * fb_info.pitch);
		row[x]	       = krgb888_to_rgb565(rgb_color);
	} else {
		kuint32_t *row = (kuint32_t *) ((kuint8_t *) framebuffer + y * fb_info.pitch);
		row[x]	       = krgb_to_bgr(rgb_color);
	}
}

static void
    kvideo_draw_glyph_at (char c, kuint32_t x, kuint32_t y, kuint32_t rgb_color) {
	if ( framebuffer == KNULL || (unsigned int) c >= 256 ) {
		return;
	}

	const unsigned char *glyph = font[(unsigned int) c];

	for ( kuint32_t row = 0; row < FONT_HEIGHT; row++ ) {
		unsigned char row_data = glyph[row];
		for ( kuint32_t col = 0; col < FONT_WIDTH; col++ ) {
			if ( row_data & (0x80 >> col) ) {
				kvideo_put_pixel(x + col, y + row, rgb_color);
			}
		}
	}
}

void
    kvideo_putchar (char c) {
	if ( !kis_video_ready() )
		return;

	if ( c == '\b' ) {
		if ( cursor_x >= FONT_WIDTH ) {
			cursor_x -= FONT_WIDTH;
		} else if ( cursor_y >= FONT_HEIGHT ) {
			cursor_y -= FONT_HEIGHT;
			cursor_x = fb_info.width - FONT_WIDTH;
		}
		// Erase the character at the current position
		for ( kuint32_t row = 0; row < FONT_HEIGHT; row++ ) {
			for ( kuint32_t col = 0; col < FONT_WIDTH; col++ ) {
				kvideo_put_pixel(cursor_x + col, cursor_y + row, 0x000000);
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
		kvideo_draw_glyph_at(c, cursor_x, cursor_y, 0xFFFFFF);
		cursor_x += FONT_WIDTH;
	}

	if ( cursor_x + FONT_WIDTH > fb_info.width ) {
		cursor_x = 0;
		cursor_y += FONT_HEIGHT;
	}

	if ( cursor_y + FONT_HEIGHT > fb_info.height ) {
		kvideo_clear(0x000000);
	}
}

void
    kvideo_draw_circle (int cx, int cy, int radius, kuint32_t rgb_color) {
	for ( int y = -radius; y <= radius; y++ ) {
		for ( int x = -radius; x <= radius; x++ ) {
			if ( x * x + y * y <= radius * radius ) {
				int px = cx + x;
				int py = cy + y;
				if ( px >= 0 && py >= 0 ) {
					kvideo_put_pixel((kuint32_t) px, (kuint32_t) py, rgb_color);
				}
			}
		}
	}
}

void
    kvideo_draw_square (int cx, int cy, int size, kuint32_t rgb_color) {
	int half = size / 2;

	for ( int y = cy - half; y < cy + half; y++ ) {
		for ( int x = cx - half; x < cx + half; x++ ) {
			if ( x >= 0 && y >= 0 && x < (int) fb_info.width
			     && y < (int) fb_info.height ) {
				kvideo_put_pixel((kuint32_t) x, (kuint32_t) y, rgb_color);
			}
		}
	}
}

void
    kvideo_puts (const char *s) {
	while ( *s ) {
		kvideo_putchar(*s++);
	}
}
