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
extern "C"
{
#endif

	extern int errno;

/* Error codes (POSIX / C Standard) */
#define ENONE	     0	/* No error information */
#define EPERM	     1	/* Operation not permitted */
#define ENOENT	     2	/* No such file or directory */
#define ESRCH	     3	/* No such process */
#define EINTR	     4	/* Interrupted system call */
#define EIO	     5	/* I/O error */
#define ENXIO	     6	/* No such device or address */
#define E2BIG	     7	/* Argument list too long */
#define ENOEXEC	     8	/* Exec format error */
#define EBADF	     9	/* Bad file number */
#define ECHILD	     10 /* No child processes */
#define EAGAIN	     11 /* Try again */
#define ENOMEM	     12 /* Out of memory */
#define EACCES	     13 /* Permission denied */
#define EFAULT	     14 /* Bad address */
#define ENOTBLK	     15 /* Block device required */
#define EBUSY	     16 /* Device or resource busy */
#define EEXIST	     17 /* File exists */
#define EXDEV	     18 /* Cross-device link */
#define ENODEV	     19 /* No such device */
#define ENOTDIR	     20 /* Not a directory */
#define EISDIR	     21 /* Is a directory */
#define EINVAL	     22 /* Invalid argument */
#define ENFILE	     23 /* File table overflow */
#define EMFILE	     24 /* Too many open files */
#define ENOTTY	     25 /* Not a typewriter */
#define ETXTBSY	     26 /* Text file busy */
#define EFBIG	     27 /* File too large */
#define ENOSPC	     28 /* No space left on device */
#define ESPIPE	     29 /* Illegal seek */
#define EROFS	     30 /* Read-only file system */
#define EMLINK	     31 /* Too many links */
#define EPIPE	     32 /* Broken pipe */
#define EDOM	     33 /* Math argument out of domain of func */
#define ERANGE	     34 /* Math result not representable */
#define EDEADLK	     35 /* Resource deadlock would occur */
#define ENAMETOOLONG 36 /* File name too long */
#define ENOLCK	     37 /* No record locks available */
#define ENOSYS	     38 /* Function not implemented */
#define ENOTEMPTY    39 /* Directory not empty */
#define ELOOP	     40 /* Too many symbolic links encountered */

/* Networking, IPC, and others (optional) */
#define EWOULDBLOCK	EAGAIN /* Operation would block */
#define ENOMSG		42     /* No message of desired type */
#define EIDRM		43     /* Identifier removed */
#define ECHRNG		44     /* Channel number out of range */
#define EL2NSYNC	45     /* Level 2 not synchronized */
#define EL3HLT		46     /* Level 3 halted */
#define EL3RST		47     /* Level 3 reset */
#define ELNRNG		48     /* Link number out of range */
#define EUNATCH		49     /* Protocol driver not attached */
#define ENOCSI		50     /* No CSI structure available */
#define EL2HLT		51     /* Level 2 halted */
#define EOVERFLOW	52     /* Value too large for defined data type */
#define EBADMSG		53     /* Bad message */
#define ENOTRECOVERABLE 54     /* State not recoverable */
#define EOWNERDEAD	55     /* Previous owner died */
#define ESTRPIPE	56     /* Streams pipe error */

/* Thread support */
#ifdef __THREAD_LOCAL_ERRNO
#undef errno
#define errno (*__errno_location())
	extern int *__errno_location (void);
#endif

#ifdef __cplusplus
}
#endif
