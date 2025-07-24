// kstrtol.c

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

#include "kstrtol.h"

#include "kctype.h"
#include "kerrno.h"
#include "klimits.h"
#include "kstdlib.h"

long
    kstrtol (const char *nptr, char **endptr, int base) {
	const char *s	= nptr;
	long	    acc = 0;
	int	    c;
	int	    neg = 0;
	long	    cutoff;
	int	    cutlim;

	// skip whitespace
	while ( kisspace((unsigned char) *s) )
		s++;

	// sign
	if ( *s == '-' ) {
		neg = 1;
		s++;
	} else if ( *s == '+' ) {
		s++;
	}

	// detect base if 0
	if ( base == 0 ) {
		if ( *s == '0' ) {
			if ( s[1] == 'x' || s[1] == 'X' ) {
				base = 16;
				s += 2;
			} else {
				base = 8;
				s++;
			}
		} else {
			base = 10;
		}
	} else if ( base == 16 ) {
		if ( s[0] == '0' && (s[1] == 'x' || s[1] == 'X') ) {
			s += 2;
		}
	}

	// compute cutoff for overflow
	cutoff = neg ? KLONG_MIN : KLONG_MAX;
	cutlim = (int) (cutoff % base);
	cutoff /= base;
	if ( cutlim < 0 ) {
		cutlim += base;
		cutoff += 1;
	}

	// convert digits
	for ( ;; s++ ) {
		c = (unsigned char) *s;
		if ( kisdigit(c) )
			c -= '0';
		else if ( kisalpha(c) )
			c = ktoupper(c) - 'A' + 10;
		else
			break;
		if ( c >= base )
			break;

		// check overflow
		if ( neg ) {
			if ( acc < cutoff || (acc == cutoff && c > cutlim) ) {
				acc    = KLONG_MIN;
				kerrno = KERANGE;
				neg    = 0; /* to avoid negating again */
			} else {
				acc *= base;
				acc -= c;
			}
		} else {
			if ( acc > cutoff || (acc == cutoff && c > cutlim) ) {
				acc    = KLONG_MAX;
				kerrno = KERANGE;
				neg    = 0; /* to avoid negating again */
			} else {
				acc *= base;
				acc += c;
			}
		}
	}

	if ( endptr )
		*endptr = (char *) (acc == 0 && s == nptr ? nptr : s);

	return acc;
}
