// ext2.h

/* ============================================================================
 * Wind Operating System / Tempest Kernel
 * Copyright (C) 2025 Wind/Tempest Foundation <https://wind.infernointeractive.win>
 * ============================================================================
 * 
 * - Overview:
 *   This source file is part of the Wind (Operating System) and Tempest (Kernel), a fully
 *   open-source operating system kernel developed to provide a robust, scalable,
 *   and efficient platform optimized for modern hardware and user-centric freedom.
 * 
 * - Project Scope:
 *   Wind/Tempest targets a modular architecture supporting symmetric multiprocessing,
 *   preemptive multitasking, fine-grained memory management, and extensible driver
 *   frameworks. The system emphasizes low latency, high throughput, and
 *   predictable behavior for both user-space applications and kernel subsystems.
 * 
 * - Licensing Terms:
 * The Wind/Tempest source code is licensed under the Liberty Software License (LSL) version 1.0,
 *   a free software license based on the GNU Affero General Public License v3 (AGPLv3)
 *   with added provisions to ensure transparency, prevent DRM, and prohibit
 *   unauthorized AI training uses.
 * 
 *   You are authorized to copy, modify, distribute, and use this software,
 *   provided that all modifications and derivatives comply with the LSL terms.
 *   Inclusion of proper attribution and licensing notices is mandatory in redistributions.
 * 
 * - Warranty and Liability Disclaimer:
 *   This software is provided "as-is", without any express or implied warranty,
 *   including but not limited to implied warranties of merchantability, fitness for
 *   a particular purpose, or non-infringement of third-party rights.
 *   Under no circumstances shall the authors, contributors, or copyright holders be liable
 *   for any damages arising directly or indirectly from the use or inability to use
 *   this software, including data loss, system failures, or other incidental damages.
 * 
 * - Contribution Guidelines:
 *   Contributions to the Wind/Tempest project are welcome and encouraged.
 *   Contributors must agree to license their contributions under the same LSL terms.
 *   When submitting patches, please ensure they adhere to the project's coding standards,
 *   include proper documentation, and maintain backward compatibility where possible.
 * 
 * - Security and Stability:
 *   The kernel maintains rigorous coding practices to minimize security vulnerabilities.
 *   Regular code audits, testing, and community reviews are integral to the project's
 *   ongoing stability and security posture.
 *   Users and developers should report issues promptly via official channels.
 * 
 * - Documentation and Support:
 *   Comprehensive documentation including API references, architecture overviews,
 *   and development guides are available on the official website:
 *     https://wind.infernointeractive.win
 *   Community support can be found through mailing lists, forums, and issue trackers
 *   hosted on the project's main repository.
 * 
 * - Additional Notes:
 *   This kernel is designed to be hardware agnostic, supporting a wide range of
 *   architectures and devices. Modularity in subsystems allows customization
 *   for embedded, desktop, and server deployments.
 * 
 * By using or modifying this code, you agree to uphold the principles of free and
 * open-source software, contributing back improvements to foster collective progress.
 */

#pragma once

#include "kstdbool.h"
#include "kstddef.h"
#include "kstdint.h"

#ifdef __cplusplus
extern "C" {
#endif

extern kbool is_mounted;

#define EXT2_SUPERBLOCK_OFFSET 1024 /* bytes from start of partition */
#define EXT2_SUPER_MAGIC       0xEF53
#define EXT2_N_BLOCKS	       15 /* Direct + indirect block ptrs */
#define EXT2_ROOT_INODE	       2

// Error codes returned by driver functions
#define EXT2_OK			0
#define EXT2_ERR_IO		-1
#define EXT2_ERR_BAD_MAGIC	-2
#define EXT2_ERR_UNSUPPORTED	-3
#define EXT2_ERR_PATH_NOT_FOUND -4
#define EXT2_ERR_INVALID	-5

// Superblock — stored at offset 1024 bytes from partition start
typedef struct __attribute__((packed)) ext2_superblock {
	kuint32_t inodes_count;	     /* Total number of inodes           */
	kuint32_t blocks_count;	     /* Total number of blocks           */
	kuint32_t r_blocks_count;    /* Reserved blocks                  */
	kuint32_t free_blocks_count; /* Free blocks                      */
	kuint32_t free_inodes_count; /* Free inodes                      */
	kuint32_t first_data_block;  /* First data block id              */
	kuint32_t log_block_size;    /* block_size = 1024 << log_bs      */
	kuint32_t log_frag_size;     /* fragment size (unused)           */
	kuint32_t blocks_per_group;
	kuint32_t frags_per_group;
	kuint32_t inodes_per_group;
	kuint32_t mtime; /* Last mount time (UNIX epoch)     */
	kuint32_t wtime; /* Last write time                  */
	kuint16_t mount_count;
	kuint16_t max_mount_count;
	kuint16_t magic; /* Should be EXT2_SUPER_MAGIC       */
	kuint16_t state;
	kuint16_t errors;
	kuint16_t minor_rev_level;
	kuint32_t lastcheck;
	kuint32_t checkinterval;
	kuint32_t creator_os;
	kuint32_t rev_level;
	kuint16_t def_resuid;
	kuint16_t def_resgid;
	// --- EXT2_DYNAMIC_REV superblock fields follow (we only need few)
	kuint32_t first_ino;  /* First non-reserved inode         */
	kuint16_t inode_size; /* Size of each inode structure     */
	kuint16_t block_group_nr;
	kuint32_t feature_compat;
	kuint32_t feature_incompat;
	kuint32_t feature_ro_compat;
	kuint8_t  uuid[16];
	char	  volume_name[16];
	char	  last_mounted[64];
	kuint32_t algorithm_usage_bitmap;
	// Fields beyond this are rarely needed for read-only support
} ext2_superblock_t;

// Block Group Descriptor (size 32 bytes)
typedef struct __attribute__((packed)) ext2_group_desc {
	kuint32_t block_bitmap; /* Block bitmap block id            */
	kuint32_t inode_bitmap; /* Inode bitmap block id            */
	kuint32_t inode_table;	/* Inode table starting block id    */
	kuint16_t free_blocks_count;
	kuint16_t free_inodes_count;
	kuint16_t used_dirs_count;
	kuint16_t pad;
	kuint32_t reserved[3];
} ext2_group_desc_t;

// Inode (dynamic size — 128 bytes for rev 0, 256+ for rev 1)
typedef struct __attribute__((packed)) ext2_inode {
	kuint16_t mode;
	kuint16_t uid;
	kuint32_t size_lo; /* Lower 32-bits of file size       */
	kuint32_t atime;
	kuint32_t ctime;
	kuint32_t mtime;
	kuint32_t dtime;
	kuint16_t gid;
	kuint16_t links_count;
	kuint32_t blocks; /* Blocks count in 512-byte units   */
	kuint32_t flags;
	kuint32_t osd1;
	kuint32_t block[EXT2_N_BLOCKS];
	kuint32_t generation;
	kuint32_t file_acl;
	kuint32_t dir_acl_or_size_high;
	kuint32_t faddr;
	kuint8_t  osd2[12];
} ext2_inode_t;

// Directory entry (variable size)
typedef struct __attribute__((packed)) ext2_dir_entry {
	kuint32_t inode;     /* Inode number                     */
	kuint16_t rec_len;   /* Directory entry length           */
	kuint8_t  name_len;  /* Name length                      */
	kuint8_t  file_type; /* EXT2_FT_...                      */
	char	  name[255]; /* File name (not null-terminated)  */
} ext2_dir_entry_t;

// File handle used by driver functions
typedef struct ext2_file {
	ext2_inode_t inode;
	kuint32_t    pos; /* Current read offset              */
} ext2_file_t;

/* --------------------------------------------------------------------------
 *  Public API (minimal)
 * ------------------------------------------------------------------------*/

// Application supplies block-device callbacks (sector-based, 512-byte)
void
    kext2_set_block_device (
	int (*read)(kuint64_t lba, kuint32_t count, void *buf),
	int (*write)(kuint64_t lba, kuint32_t count, const void *buf));

// Mount an EXT2 filesystem located at @base_lba. Returns EXT2_OK on success.
int
    kext2_mount (kuint64_t base_lba);

// Open file by absolute POSIX path (e.g., "/etc/issue"). Read-only.
int
    kext2_open (const char *path, ext2_file_t *out_file);

// Read up to @len bytes from file into @buf starting at current position.
int
    kext2_read (ext2_file_t *file, void *buf, ksize_t len);

// Callback type for listing directory entries
typedef void (*ext2_list_cb_t)(const char *name, kuint8_t file_type);

// List entries in the directory specified by POSIX path (e.g. "/System/fonts").
int
    kext2_list (const char *path, ext2_list_cb_t cb);

#ifdef __cplusplus
}
#endif
