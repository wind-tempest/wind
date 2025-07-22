/* dlog.c */

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

#include "core/kernel.h"
#include "dlog.h"
#include "drivers/serial/serial.h"
#include "kprint.h"
#include "kstdarg.h"
#include "kutoa.h"

void
    kduts (const char *s) {
	if ( !kuse_debug ) {
		return;
	}
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
				/* Handle long/long long modifiers */
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
				/* Fall through to default for single 'l' */
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
