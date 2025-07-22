/* kerrno.h */

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

#ifdef __cplusplus
extern "C" {
#endif

extern int kerrno;

/* Error codes (POSIX / C Standard) */
#define KENONE	      0	 /* No error information */
#define KEPERM	      1	 /* Operation not permitted */
#define KENOENT	      2	 /* No such file or directory */
#define KESRCH	      3	 /* No such process */
#define KEINTR	      4	 /* Interrupted system call */
#define KEIO	      5	 /* I/O error */
#define KENXIO	      6	 /* No such device or address */
#define KE2BIG	      7	 /* Argument list too long */
#define KENOEXEC      8	 /* Exec format error */
#define KEBADF	      9	 /* Bad file number */
#define KECHILD	      10 /* No child processes */
#define KEAGAIN	      11 /* Try again */
#define KENOMEM	      12 /* Out of memory */
#define KEACCES	      13 /* Permission denied */
#define KEFAULT	      14 /* Bad address */
#define KENOTBLK      15 /* Block device required */
#define KEBUSY	      16 /* Device or resource busy */
#define KEEXIST	      17 /* File exists */
#define KEXDEV	      18 /* Cross-device link */
#define KENODEV	      19 /* No such device */
#define KENOTDIR      20 /* Not a directory */
#define KEISDIR	      21 /* Is a directory */
#define KEINVAL	      22 /* Invalid argument */
#define KENFILE	      23 /* File table overflow */
#define KEMFILE	      24 /* Too many open files */
#define KENOTTY	      25 /* Not a typewriter */
#define KETXTBSY      26 /* Text file busy */
#define KEFBIG	      27 /* File too large */
#define KENOSPC	      28 /* No space left on device */
#define KESPIPE	      29 /* Illegal seek */
#define KEROFS	      30 /* Read-only file system */
#define KEMLINK	      31 /* Too many links */
#define KEPIPE	      32 /* Broken pipe */
#define KEDOM	      33 /* Math argument out of domain of func */
#define KERANGE	      34 /* Math result not representable */
#define KEDEADLK      35 /* Resource deadlock would occur */
#define KENAMETOOLONG 36 /* File name too long */
#define KENOLCK	      37 /* No record locks available */
#define KENOSYS	      38 /* Function not implemented */
#define KENOTEMPTY    39 /* Directory not empty */
#define KELOOP	      40 /* Too many symbolic links encountered */

/* Networking, IPC, and others (optional) */
#define KEWOULDBLOCK	 EAGAIN /* Operation would block */
#define KENOMSG		 42	/* No message of desired type */
#define KEIDRM		 43	/* Identifier removed */
#define KECHRNG		 44	/* Channel number out of range */
#define KEL2NSYNC	 45	/* Level 2 not synchronized */
#define KEL3HLT		 46	/* Level 3 halted */
#define KEL3RST		 47	/* Level 3 reset */
#define KELNRNG		 48	/* Link number out of range */
#define KEUNATCH	 49	/* Protocol driver not attached */
#define KENOCSI		 50	/* No CSI structure available */
#define KEL2HLT		 51	/* Level 2 halted */
#define KEOVERFLOW	 52	/* Value too large for defined data type */
#define KEBADMSG	 53	/* Bad message */
#define KENOTRECOVERABLE 54	/* State not recoverable */
#define KEOWNERDEAD	 55	/* Previous owner died */
#define KESTRPIPE	 56	/* Streams pipe error */

/* Thread support */
#ifdef K__THREAD_LOCAL_ERRNO
#	undef errno
#	define errno (*K__errno_location())
extern int *
    K__errno_location (void);
#endif

#ifdef __cplusplus
}
#endif
