/* kerrno.h */

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
