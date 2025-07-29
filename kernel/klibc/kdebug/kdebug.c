// kdebug.c

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.infernointeractive.win>
 *
 * This file is part of the Wind Operating System and Tempest Kernel.
 * It is licensed under the GNU General Public License version 3.0  (GPLv3).
 */

#include "core/entry/kernel.h"
#include "drivers/serial/serial.h"

#include <wt/kdebug/kdebug.h>
#include <wt/kstdarg.h>
#include <wt/kstdio/kprint/kprint.h>
#include <wt/kstdio/kstrlen/kstrlen.h>
#include <wt/kstdlib/kutoa.h>
#include <wt/kstring/kstrcmp.h>

/*
 * This debugging tool is only for early days of development. It is not
 * gonna be on the final release (AKA: v1.0.0) of the Wind Operating System.
 * /Tempest Kernel.
 */

const char *debug_type_message = "[DEBUG] ";

void
    kduts (const char *s) {
	if ( !kuse_debug ) {
		return;
	}
	if ( !s || *s == '\0' )
		return;
	serial_writes(debug_type_message);
	serial_writes(s);
	serial_write('\n');
}

int
    kdebugf (const char *format, ...) {
	if ( !kuse_debug )
		return 0;

	if ( !format || *format == '\0' )
		return 1;

	va_list args;
	k_va_start(args, format);
	int count = 0;

	serial_writes(debug_type_message);
	count += (int) kstrlen(debug_type_message);

	for ( const char *p = format; *p; ++p ) {
		if ( *p != '%' ) {
			serial_write(*p);
			count++;
			continue;
		}

		p++;

		int left_align = 0;
		int width      = 0;

		if ( *p == '-' ) {
			left_align = 1;
			p++;
		}

		while ( *p >= '0' && *p <= '9' ) {
			width = width * 10 + (*p - '0');
			p++;
		}

		switch ( *p ) {
			case 's': {
				const char *s	= k_va_arg(args, const char *);
				int	    len = 0;
				const char *t	= s;
				while ( *t++ )
					len++;

				int pad = (width > len) ? (width - len) : 0;

				if ( !left_align ) {
					for ( int i = 0; i < pad; ++i ) {
						serial_write(' ');
						count++;
					}
				}

				serial_writes(s);
				count += len;

				if ( left_align ) {
					for ( int i = 0; i < pad; ++i ) {
						serial_write(' ');
						count++;
					}
				}
				break;
			}

			case 'd': {
				int   n = k_va_arg(args, int);
				char  buf[12];
				char *ptr = buf;

				if ( n < 0 ) {
					*ptr++ = '-';
					n      = -n;
				}

				char *end_ptr = kutoa(
				    ptr, buf + sizeof(buf) - 1, (unsigned int) n, 10, 0);
				*end_ptr = '\0';
				serial_writes(buf);
				count += (int) (end_ptr - buf);
				break;
			}

			case 'x': {
				unsigned int n = k_va_arg(args, unsigned int);
				char	     buf[12];
				char	    *end_ptr =
				    kutoa(buf, buf + sizeof(buf) - 1, n, 16, 0);
				*end_ptr = '\0';
				serial_writes(buf);
				count += (int) (end_ptr - buf);
				break;
			}
			case 'l': {
				// Handle long/long long modifiers
				if ( *(p + 1) == 'l' ) {
					p++; // Skip second 'l'
					if ( *(p + 1) == 'x' ) {
						p++; // Skip 'x'
						kuint64_t n = k_va_arg(args, kuint64_t);
						char	  buf[20];
						char	 *end_ptr = kutoa(
							buf,
							buf + sizeof(buf) - 1,
							(unsigned int) (n & 0xFFFFFFFF),
							16,
							0);
						*end_ptr = '\0';
						serial_writes(buf);
						count += (int) (end_ptr - buf);
						break;
					}
				}
				// Fall through to default for single 'l'
				goto default_case;
			}

			case 'c': {
				char c = (char) k_va_arg(args, int);
				serial_write(c);
				count++;
				break;
			}

			case '%': {
				serial_write('%');
				count++;
				break;
			}

			default:
			default_case: {
				serial_write('%');
				serial_write(*p);
				count += 2;
				break;
			}
		}
	}

	k_va_end(args);
	return count;
}

void
    kdbgtype (const char *type, const char *message, const char *extra) {
	if ( !message || *message == '\0' )
		return;
	if ( !extra || *extra == '\0' ) {
		kprintf("[%s] %s\n", type, message);
	} else {
		kprintf("[%s] %s: %s\n", type, message, extra);
	}
}

void
    kcritical (const char *message, const char *extra) {
	kdbgtype("CRITICAL", message, extra);
}

void
    kalert (const char *message, const char *extra) {
	kdbgtype("ALERT", message, extra);
}

void
    kemerg (const char *message, const char *extra) {
	kdbgtype("EMERG", message, extra);
}

void
    kwarn (const char *message, const char *extra) {
	kdbgtype("WARN", message, extra);
}

void
    kerror (const char *message, const char *extra) {
	kdbgtype("ERROR", message, extra);
}

void
    knotice (const char *message, const char *extra) {
	kdbgtype("NOTICE", message, extra);
}

void
    kinfo (const char *message, const char *extra) {
	kdbgtype("INFO", message, extra);
}
