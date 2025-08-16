// SPDX-License-Identifier: LSL-1.2-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */

#include "core/init/main.h"

#include <tempest/drivers/serial.h>
#include <tempest/klibc/kdebug/kdebug.h>
#include <tempest/klibc/kstdarg.h>
#include <tempest/klibc/kstdio/kprint/kprint.h>
#include <tempest/klibc/kstdio/kstrlen/kstrlen.h>
#include <tempest/klibc/kstdlib/kutoa.h>
#include <tempest/klibc/kstring/kstrcmp.h>

/*
 * This debugging tool is only for early days of development. It is not
 * gonna be on the final release (AKA: v1.0.0) of the Wind (Operating System)
 * or Tempest (the Kernel).
 */

const char *debug_type_message = "[    debug] ";

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
					p++;  // Skip second 'l'
					if ( *(p + 1) == 'x' ) {
						p++;  // Skip 'x'
						kuint64_t n = k_va_arg(args, kuint64_t);
						char	  buf[20];
						char	 *end_ptr =
						    kutoa(buf,
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
    kdbgtype (const char *type,
	      const char *subsystem,
	      const char *message,
	      const char *extra) {
	if ( !message || *message == '\0' )
		return;

	kprintf("[    %s", type);

	if ( subsystem && *subsystem != '\0' )
		kprintf("::%s", subsystem);

	kprintf("] %s", message);

	if ( extra && *extra != '\0' )
		kprintf(": %s", extra);

	kprintf("\n");
}

void
    kcritical (const char *message, const char *subsystem, const char *extra) {
	kdbgtype("crit", subsystem, message, extra);
}

void
    kalert (const char *message, const char *subsystem, const char *extra) {
	kdbgtype("alert", subsystem, message, extra);
}

void
    kemerg (const char *message, const char *subsystem, const char *extra) {
	kdbgtype("emerg", subsystem, message, extra);
}

void
    kwarn (const char *message, const char *subsystem, const char *extra) {
	kdbgtype("warn", subsystem, message, extra);
}

void
    kerror (const char *message, const char *subsystem, const char *extra) {
	kdbgtype("err", subsystem, message, extra);
}

void
    knotice (const char *message, const char *subsystem, const char *extra) {
	kdbgtype("notice", subsystem, message, extra);
}

void
    kinfo (const char *message, const char *subsystem, const char *extra) {
	kdbgtype("info", subsystem, message, extra);
}
