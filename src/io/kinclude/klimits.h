/* klimits.h */

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

#pragma once

/* Number of bits in a `char`. */
#define CHAR_BIT 8

/* Minimum and maximum values for signed char. */
#define SCHAR_MIN (-128)
#define SCHAR_MAX 127

/* Minimum and maximum values for char. */
#if defined(__CHAR_UNSIGNED__)
#	define CHAR_MIN 0
#	define CHAR_MAX 255
#else
#	define CHAR_MIN SCHAR_MIN
#	define CHAR_MAX SCHAR_MAX
#endif

/* Maximum value for unsigned char. */
#define KUCHAR_MAX 255

/* Maximum value for multibyte character length. */
#define KMB_LEN_MAX 16

/* Minimum and maximum values for short. */
#define KSHRT_MIN  (-32768)
#define KSHRT_MAX  32767
#define KUSHRT_MAX 65535U

/* Minimum and maximum values for int. */
#define KINT_MIN  (-2147483647 - 1)
#define KINT_MAX  2147483647
#define KUINT_MAX 4294967295U

/* Minimum and maximum values for long. */
#if defined(_LP64) || defined(__x86_64__)
/* LP64: long is 64 bits */
#	define KLONG_MIN  (-9223372036854775807L - 1)
#	define KLONG_MAX  9223372036854775807L
#	define KULONG_MAX 18446744073709551615UL
#else
/* ILP32: long is 32 bits */
#	define KLONG_MIN  (-2147483647L - 1)
#	define KLONG_MAX  2147483647L
#	define KULONG_MAX 4294967295UL
#endif

/* Minimum and maximum values for long long. */
#define KLLONG_MIN  (-9223372036854775807LL - 1)
#define KLLONG_MAX  9223372036854775807LL
#define KULLONG_MAX 18446744073709551615ULL

/* Additional limits (C99 and POSIX) */

/* Pointer difference type */
#define KPTRDIFF_MIN (-KINT_MAX - 1)
#define KPTRDIFF_MAX KINT_MAX

/* Size type */
#define KSIZE_MAX KUINT_MAX

/* sig_atomic_t */
#define KSIG_ATOMIC_MIN (-2147483647 - 1)
#define KSIG_ATOMIC_MAX 2147483647

/* wchar_t and wint_t */
#define KWCHAR_MIN 0
#define KWCHAR_MAX 4294967295U
#define KWINT_MIN  0u
#define KWINT_MAX  4294967295u
