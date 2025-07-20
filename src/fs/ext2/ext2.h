/* ext2.h */

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

/* Error codes returned by driver functions                                                 */
#define EXT2_OK			0
#define EXT2_ERR_IO		-1
#define EXT2_ERR_BAD_MAGIC	-2
#define EXT2_ERR_UNSUPPORTED	-3
#define EXT2_ERR_PATH_NOT_FOUND -4
#define EXT2_ERR_INVALID	-5

/* Superblock — stored at offset 1024 bytes from partition start           */
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
	/* --- EXT2_DYNAMIC_REV superblock fields follow (we only need few) */
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
	/* Fields beyond this are rarely needed for read-only support */
} ext2_superblock_t;

/* Block Group Descriptor (size 32 bytes)                                   */
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

/* Inode (dynamic size — 128 bytes for rev 0, 256+ for rev 1)               */
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

/* Directory entry (variable size)                                          */
typedef struct __attribute__((packed)) ext2_dir_entry {
	kuint32_t inode;     /* Inode number                     */
	kuint16_t rec_len;   /* Directory entry length           */
	kuint8_t  name_len;  /* Name length                      */
	kuint8_t  file_type; /* EXT2_FT_...                      */
	char	  name[255]; /* File name (not null-terminated)  */
} ext2_dir_entry_t;

/* File handle used by driver functions                                     */
typedef struct ext2_file {
	ext2_inode_t inode;
	kuint32_t    pos; /* Current read offset              */
} ext2_file_t;

/* --------------------------------------------------------------------------
 *  Public API (minimal)
 * ------------------------------------------------------------------------*/

/* Application supplies block-device callbacks (sector-based, 512-byte) */
void
    kext2_set_block_device (
	int (*read)(kuint64_t lba, kuint32_t count, void *buf),
	int (*write)(kuint64_t lba, kuint32_t count, const void *buf));

/* Mount an EXT2 filesystem located at @base_lba. Returns EXT2_OK on success. */
int
    kext2_mount (kuint64_t base_lba);

/* Open file by absolute POSIX path (e.g., "/etc/issue"). Read-only.       */
int
    kext2_open (const char *path, ext2_file_t *out_file);

/* Read up to @len bytes from file into @buf starting at current position.  */
int
    kext2_read (ext2_file_t *file, void *buf, ksize_t len);

/* Callback type for listing directory entries */
typedef void (*ext2_list_cb_t)(const char *name, kuint8_t file_type);

/* List entries in the directory specified by POSIX path (e.g. "/System/fonts"). */
int
    kext2_list (const char *path, ext2_list_cb_t cb);

#ifdef __cplusplus
}
#endif
