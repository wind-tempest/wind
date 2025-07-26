// kdebug.c

// SPDX-License-Identifier: LSL-1.0
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.infernointeractive.win>
 *
 * This file is part of the Wind Operating System and Tempest Kernel.
 * It is licensed under the Liberty Software License 1.0 (LSL-1.0).
 * For more information, see <https://wind.infernointeractive.win>.
 */

#include "kdebug.h"

#include "core/entry/kernel.h"
#include "drivers/serial/serial.h"
#include "io/kstdio/kstrlen/kstrlen.h"
#include "kprint.h"
#include "kstdarg.h"
#include "kutoa.h"

void
    kduts (const char *s) {
	if ( !kuse_debug ) {
		return;
	}
	serial_writes("[DEBUG] ");
	serial_writes(s);
	serial_write('\n');
}

int
    kdebugf (const char *format, ...) {
	if ( !kuse_debug )
		return 0;

	va_list args;
	k_va_start(args, format);
	int count = 0;

	const char debug_message[] = "[DEBUG] ";
	serial_writes(debug_message);
	count += (int) kstrlen(debug_message);

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

				char *end_ptr =
				    kutoa(ptr, buf + sizeof(buf) - 1, (unsigned int) n, 10, 0);
				*end_ptr = '\0';
				serial_writes(buf);
				count += (int) (end_ptr - buf);
				break;
			}

			case 'x': {
				unsigned int n = k_va_arg(args, unsigned int);
				char	     buf[12];
				char	    *end_ptr = kutoa(buf, buf + sizeof(buf) - 1, n, 16, 0);
				*end_ptr	     = '\0';
				serial_writes(buf);
				count += (int) (end_ptr - buf);
				break;
			}
			case 'l': {
				// Handle long/long long modifiers
				if ( *(p + 1) == 'l' ) {
					p++; /* Skip second 'l' */
					if ( *(p + 1) == 'x' ) {
						p++; /* Skip 'x' */
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
    kcritical (const char *message, const char *extra) {
	if ( extra == KNULL ) {
		kprintf("[CRITICAL] %s\n", message);
	} else {
		kprintf("[CRITICAL] %s: %s\n", message, extra);
	}
}

void
    kalert (const char *message, const char *extra) {
	if ( extra == KNULL ) {
		kprintf("[ALERT] %s\n", message);
	} else {
		kprintf("[ALERT] %s: %s\n", message, extra);
	}
}

void
    kemerg (const char *message, const char *extra) {
	if ( extra == KNULL ) {
		kprintf("[EMERGENCY] %s\n", message);
	} else {
		kprintf("[EMERGENCY] %s: %s\n", message, extra);
	}
}

void
    kwarn (const char *message, const char *extra) {
	if ( extra == KNULL ) {
		kprintf("[WARN] %s\n", message);
	} else {
		kprintf("[WARN] %s: %s\n", message, extra);
	}
}

void
    kerror (const char *message, const char *extra) {
	if ( extra == KNULL ) {
		kprintf("[ERROR] %s\n", message);
	} else {
		kprintf("[ERROR] %s: %s\n", message, extra);
	}
}

void
    knotice (const char *message, const char *extra) {
	if ( extra == KNULL ) {
		kprintf("[NOTICE] %s\n", message);
	} else {
		kprintf("[NOTICE] %s: %s\n", message, extra);
	}
}

void
    kinfo (const char *message, const char *extra) {
	if ( extra == KNULL ) {
		kprintf("[INFO] %s\n", message);
	} else {
		kprintf("[INFO] %s: %s\n", message, extra);
	}
}
