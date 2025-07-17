/* ext2.c */

/*
 * Copyright (C) 2025 Wind/Tempest Foundation
 *
 * This file is part of Wind/Tempest.
 *
 * Wind/Tempest is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Wind/Tempest is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "core/memory/memory.h"
#include "ext2.h"
#include "kstdio.h"
#include "kstring.h"

static uint64_t		  g_base_lba = 0; /* Partition/LBA base of filesystem         */
static ext2_superblock_t  g_superblock;
static ext2_group_desc_t *g_group_desc = NULL;
static uint32_t		  g_block_size = 0;
bool			  is_mounted   = false;

/* Caller-supplied I/O functions (sector based, 512-byte) */
static int (*g_read_sector)(uint64_t lba, uint32_t count, void *buf)	    = NULL;
static int (*g_write_sector)(uint64_t lba, uint32_t count, const void *buf) = NULL;

/* Convenience wrapper: read N sectors into dst */
static int
    read_sectors (uint64_t lba, uint32_t cnt, void *dst)
{
	if ( !g_read_sector )
	{
		return EXT2_ERR_IO;
	}
	return g_read_sector(lba, cnt, dst);
}

/* Read an aligned filesystem block into dst. Assumes dst has at least block_size bytes. */
static int
    read_block (uint32_t block_id, void *dst)
{
	uint64_t first_lba = g_base_lba + ((uint64_t) block_id * g_block_size) / 512;
	uint32_t cnt	   = g_block_size / 512;
	return read_sectors(first_lba, cnt, dst);
}

/* Forward declaration for functions used before definition */
static int
    read_inode (uint32_t ino, ext2_inode_t *out);

void
    ext2_set_block_device (int (*read)(uint64_t, uint32_t, void *),
			   int (*write)(uint64_t, uint32_t, const void *))
{
	g_read_sector  = read;
	g_write_sector = write; /* May be NULL for read-only volumes */
}

int
    ext2_mount (uint64_t base_lba)
{
	if ( !g_read_sector )
	{
		return EXT2_ERR_IO;
	}

	g_base_lba = base_lba;

	/* Read raw 1024-byte superblock into temp buffer and copy only the
	 * structure portion we need. This avoids overflowing g_superblock
	 * (struct is ~128 bytes). */
	uint8_t sb_raw[1024];
	if ( read_sectors(base_lba + EXT2_SUPERBLOCK_OFFSET / 512, 2, sb_raw) != 0 )
	{
		return EXT2_ERR_IO;
	}
	kmemcpy(&g_superblock, sb_raw, sizeof(ext2_superblock_t));

	if ( g_superblock.magic != EXT2_SUPER_MAGIC )
	{
#ifdef EXT2_DEBUG
		kprintf("EXT2: bad magic 0x%04x (expected 0x%04x)\n",
			g_superblock.magic,
			EXT2_SUPER_MAGIC);
#endif
		return EXT2_ERR_BAD_MAGIC;
	}

	/* Compute block size and validate (EXT2 supports 1K,2K,4K). Anything
	 * larger is non-standard and can easily blow up our simple kmalloc.
	 * Reject filesystems with block sizes >4 KiB. */
	if ( g_superblock.log_block_size > 2 )
	{
#ifdef EXT2_DEBUG
		kprintf("EXT2: unsupported block size (log=%u)\n", g_superblock.log_block_size);
#endif
		return EXT2_ERR_UNSUPPORTED;
	}
	g_block_size = 1024U << g_superblock.log_block_size;

	uint32_t groups = (g_superblock.blocks_count + g_superblock.blocks_per_group - 1)
			  / g_superblock.blocks_per_group;

	size_t gd_table_bytes  = groups * sizeof(ext2_group_desc_t);
	size_t gd_table_blocks = (gd_table_bytes + g_block_size - 1) / g_block_size;

	g_group_desc = (ext2_group_desc_t *) kmalloc(gd_table_blocks * g_block_size);
	if ( !g_group_desc )
	{
		return EXT2_ERR_IO;
	}

	/* Group descriptor table starts right after superblock (block 2 for 1K blk) */
	/* For 1 KiB block size the layout is: block0=boot, block1=superblock,
	 * block2=group descriptor table. For 2 KiB/4 KiB, superblock is at
	 * byte 1024 inside block0, so the descriptor table immediately follows
	 * in block1.
	 */
	uint32_t gd_start_blk = (g_block_size == 1024) ? 2 : 1;
	for ( uint32_t i = 0; i < gd_table_blocks; i++ )
	{
		if ( read_block(gd_start_blk + i, (uint8_t *) g_group_desc + i * g_block_size)
		     != 0 )
		{
			return EXT2_ERR_IO;
		}
	}

#ifdef EXT2_DEBUG
	kprintf("EXT2: mounted. Block size %u, groups %u\n", g_block_size, groups);
#endif
	return EXT2_OK;
}

int
    ext2_open (const char *path, ext2_file_t *out_file)
{
	(void) path;
	(void) out_file;
	if ( !path || !out_file )
		return EXT2_ERR_INVALID;

	/* If path is just "/" open root directory */
	if ( kstrcmp(path, "/") == 0 )
	{
		if ( read_inode(EXT2_ROOT_INODE, &out_file->inode) != EXT2_OK )
			return EXT2_ERR_IO;
		out_file->pos = 0;
		return EXT2_OK;
	}

	/* Make a modifiable copy of path without leading '/' */
	char   tmp[256];
	size_t plen = (size_t) kstrlen(path);
	if ( plen >= sizeof(tmp) )
		return EXT2_ERR_INVALID;
	kstrcpy(tmp, path[0] == '/' ? path + 1 : path);

	/* Current inode when traversing */
	ext2_inode_t cur_inode;
	if ( read_inode(EXT2_ROOT_INODE, &cur_inode) != EXT2_OK )
		return EXT2_ERR_IO;

	char *tok = kstrtok(tmp, "/");
	char *next_tok;
	while ( tok )
	{
		next_tok = kstrtok(NULL, "/");

		/* Search for tok in cur_inode directory entries (direct blocks only) */
		uint32_t found_ino = 0;

		uint8_t *blk_buf = kmalloc(g_block_size);
		if ( !blk_buf )
			return EXT2_ERR_IO;

		for ( int i = 0; i < 12 && found_ino == 0; i++ )
		{
			uint32_t blk = cur_inode.block[i];
			if ( blk == 0 )
				continue;
			if ( read_block(blk, blk_buf) != 0 )
			{
				kfree(blk_buf);
				return EXT2_ERR_IO;
			}
			uint32_t off = 0;
			while ( off < g_block_size )
			{
				ext2_dir_entry_t *ent = (ext2_dir_entry_t *) (blk_buf + off);
				if ( ent->rec_len == 0 )
					break;
				if ( ent->inode != 0 && ent->name_len == (uint8_t) kstrlen(tok)
				     && kmemcmp(ent->name, tok, ent->name_len) == 0 )
				{
					found_ino = ent->inode;
					break;
				}
				off += ent->rec_len;
			}
		}
		kfree(blk_buf);
		if ( found_ino == 0 )
			return EXT2_ERR_PATH_NOT_FOUND;

		/* Load inode */
		if ( read_inode(found_ino, &cur_inode) != EXT2_OK )
			return EXT2_ERR_IO;

		if ( next_tok )
		{
			/* Expect directory */
			if ( !(cur_inode.mode & 0x4000) )
				return EXT2_ERR_INVALID; /* Not a directory */
		}
		else
		{
			/* Last component - should be file (or directory, we allow both) */
			out_file->inode = cur_inode;
			out_file->pos	= 0;
			return EXT2_OK;
		}

		tok = next_tok;
	}
	return EXT2_ERR_INVALID;
}

static int
    read_inode (uint32_t ino, ext2_inode_t *out)
{
	if ( ino == 0 )
		return EXT2_ERR_INVALID;

	uint32_t idx0  = ino - 1;
	uint32_t group = idx0 / g_superblock.inodes_per_group;
	uint32_t index = idx0 % g_superblock.inodes_per_group;

	if ( !g_group_desc )
		return EXT2_ERR_INVALID;

	uint32_t inode_tbl_blk = g_group_desc[group].inode_table;
	uint32_t inode_size    = g_superblock.inode_size ? g_superblock.inode_size : 128;

	uint64_t offset	      = (uint64_t) index * inode_size;
	uint32_t blk_offset   = (uint32_t) (offset / g_block_size);
	uint32_t off_in_block = (uint32_t) (offset % g_block_size);

	uint8_t *tmp = kmalloc(g_block_size);
	if ( !tmp )
		return EXT2_ERR_IO;

	/* Read the first block that contains (part of) the inode */
	if ( read_block(inode_tbl_blk + blk_offset, tmp) != 0 )
	{
		kfree(tmp);
		return EXT2_ERR_IO;
	}

	uint32_t bytes_first = g_block_size - off_in_block;
	uint32_t need	     = sizeof(ext2_inode_t);

	if ( bytes_first >= need )
	{
		/* Inode fits entirely in this block */
		kmemcpy(out, tmp + off_in_block, need);
	}
	else
	{
		/* Copy first portion */
		kmemcpy(out, tmp + off_in_block, bytes_first);

		/* Read next block for the remaining bytes */
		if ( read_block(inode_tbl_blk + blk_offset + 1, tmp) != 0 )
		{
			kfree(tmp);
			return EXT2_ERR_IO;
		}
		kmemcpy((uint8_t *) out + bytes_first, tmp, need - bytes_first);
	}

	kfree(tmp);
	return EXT2_OK;
}

static int
    list_dir_entries (const ext2_inode_t *dir_inode, ext2_list_cb_t cb)
{
	if ( !dir_inode || !cb )
		return EXT2_ERR_INVALID;

	if ( !(dir_inode->mode & 0x4000) ) /* not a directory */
		return EXT2_ERR_INVALID;

	uint8_t *block_buf = kmalloc(g_block_size);
	if ( !block_buf )
		return EXT2_ERR_IO;

	for ( int i = 0; i < 12; i++ )
	{
		uint32_t blk_id = dir_inode->block[i];
		if ( blk_id == 0 )
			continue;
		if ( read_block(blk_id, block_buf) != 0 )
		{
			kfree(block_buf);
			return EXT2_ERR_IO;
		}

		uint32_t off = 0;
		while ( off < g_block_size )
		{
			ext2_dir_entry_t *ent = (ext2_dir_entry_t *) (block_buf + off);

			uint16_t rec_len = ent->rec_len;
			uint8_t	 nlen	 = ent->name_len;
			/* Basic sanity checks */
			if ( rec_len < 8 || rec_len > g_block_size - off )
				break;
			if ( ent->inode != 0 && nlen == 0 )
				break;
			uint16_t min_len = (uint16_t) (((8 + nlen + 3) / 4) * 4);
			if ( rec_len < min_len )
				break;

			if ( ent->inode != 0 && nlen > 0 )
			{
				char name[256] = {0};
				kmemcpy(name, ent->name, nlen);
				name[nlen] = '\0';
				cb(name, ent->file_type);
			}
			off += rec_len;
		}
	}
	kfree(block_buf);
	return EXT2_OK;
}

int
    ext2_list (const char *path, ext2_list_cb_t cb)
{
	if ( !path || !cb )
		return EXT2_ERR_INVALID;

	/* Special case: root directory */
	if ( kstrcmp(path, "/") == 0 || path[0] == '\0' )
	{
		ext2_inode_t root;
		if ( read_inode(EXT2_ROOT_INODE, &root) != EXT2_OK )
			return EXT2_ERR_IO;
		if ( !(root.mode & 0x4000) )
			return EXT2_ERR_INVALID;
		return list_dir_entries(&root, cb);
	}

	char   tmp[256];
	size_t plen = (size_t) kstrlen(path);
	if ( plen >= sizeof(tmp) )
		return EXT2_ERR_INVALID;
	kstrcpy(tmp, path[0] == '/' ? path + 1 : path);

	ext2_inode_t cur_inode;
	if ( read_inode(EXT2_ROOT_INODE, &cur_inode) != EXT2_OK )
		return EXT2_ERR_IO;

	char *tok = kstrtok(tmp, "/");
	char *next_tok;
	while ( tok )
	{
		next_tok = kstrtok(NULL, "/");

		uint32_t found_ino = 0;
		uint8_t *blk_buf   = kmalloc(g_block_size);
		if ( !blk_buf )
			return EXT2_ERR_IO;

		for ( int i = 0; i < 12 && found_ino == 0; i++ )
		{
			uint32_t blk = cur_inode.block[i];
			if ( blk == 0 )
				continue;
			if ( read_block(blk, blk_buf) != 0 )
			{
				kfree(blk_buf);
				return EXT2_ERR_IO;
			}
			uint32_t off = 0;
			while ( off < g_block_size )
			{
				ext2_dir_entry_t *ent = (ext2_dir_entry_t *) (blk_buf + off);
				if ( ent->rec_len == 0 )
					break;
				if ( ent->inode != 0 && ent->name_len == (uint8_t) kstrlen(tok)
				     && kmemcmp(ent->name, tok, ent->name_len) == 0 )
				{
					found_ino = ent->inode;
					break;
				}
				off += ent->rec_len;
			}
		}
		kfree(blk_buf);
		if ( found_ino == 0 )
			return EXT2_ERR_PATH_NOT_FOUND;

		if ( read_inode(found_ino, &cur_inode) != EXT2_OK )
			return EXT2_ERR_IO;

		if ( next_tok )
		{
			if ( !(cur_inode.mode & 0x4000) )
				return EXT2_ERR_INVALID;
		}
		else
		{
			if ( !(cur_inode.mode & 0x4000) )
				return EXT2_ERR_INVALID;
			return list_dir_entries(&cur_inode, cb);
		}

		tok = next_tok;
	}
	return EXT2_ERR_INVALID;
}

int
    ext2_list_dir (ext2_list_cb_t cb)
{
	if ( !cb )
		return EXT2_ERR_INVALID;

	ext2_inode_t root;
	if ( read_inode(EXT2_ROOT_INODE, &root) != EXT2_OK )
		return EXT2_ERR_IO;

	if ( !(root.mode & 0x4000) ) /* not a directory */
		return EXT2_ERR_INVALID;

	uint8_t *block_buf = kmalloc(g_block_size);
	if ( !block_buf )
		return EXT2_ERR_IO;

	/* --- Direct blocks --- */
	for ( int i = 0; i < 12; i++ ) /* Only direct blocks for now */
	{
		uint32_t blk_id = root.block[i];
		if ( blk_id == 0 )
			continue;
		if ( read_block(blk_id, block_buf) != 0 )
		{
			kfree(block_buf);
			return EXT2_ERR_IO;
		}

		uint32_t off = 0;
		while ( off < g_block_size )
		{
			ext2_dir_entry_t *ent = (ext2_dir_entry_t *) (block_buf + off);
			/* Sanity checks to avoid malformed directory entries
			 * causing memory corruption or endless loops. Conditions:
			 *  - rec_len >= 8 (fixed-size part of dir entry)
			 *  - name_len <= 255 and name_len > 0 when inode != 0
			 *  - rec_len >= 8 + name_len, padded to 4-byte boundary
			 *  - rec_len keeps us within the current block
			 * If any condition fails, stop processing this block. */
			uint16_t rec_len = ent->rec_len;
			uint8_t	 nlen	 = ent->name_len;
			if ( rec_len < 8 || rec_len > g_block_size - off )
				break; /* Bad length */
			if ( ent->inode != 0 && nlen == 0 )
				break; /* Invalid */
			/* 4-byte aligned length of header + name */
			uint16_t min_len = (uint16_t) (((8 + nlen + 3) / 4) * 4);
			if ( rec_len < min_len )
				break; /* Corrupt */

			if ( ent->inode != 0 && nlen > 0 )
			{
				char name[256] = {0};
				kmemcpy(name, ent->name, nlen);
				name[nlen] = '\0';
				cb(name, ent->file_type);
			}
			off += rec_len;
		}
	}
	kfree(block_buf);
	return EXT2_OK;
}

int
    ext2_read (ext2_file_t *file, void *buf, size_t len)
{
	if ( !file || !buf )
		return EXT2_ERR_INVALID;

	/* Clamp length to remaining bytes in file */
	uint32_t size = file->inode.size_lo; /* We only support files <4GiB */
	if ( file->pos >= size )
		return 0; /* EOF */
	if ( file->pos + len > size )
		len = size - file->pos;

	uint8_t *dst	   = (uint8_t *) buf;
	size_t	 remaining = len;

	uint8_t *block_buf = kmalloc(g_block_size);
	if ( !block_buf )
		return EXT2_ERR_IO;

	while ( remaining > 0 )
	{
		uint32_t block_idx    = file->pos / g_block_size;
		uint32_t off_in_block = file->pos % g_block_size;
		if ( block_idx >= 12 )
		{
			/* Indirect blocks not supported yet */
			kfree(block_buf);
			return EXT2_ERR_UNSUPPORTED;
		}
		uint32_t blk_id = file->inode.block[block_idx];
		if ( blk_id == 0 )
		{
			/* Sparse region – treat as zero */
			size_t chunk = g_block_size - off_in_block;
			if ( chunk > remaining )
				chunk = remaining;
			memset(dst, 0, chunk);
			dst += chunk;
			file->pos += (uint32_t) chunk;
			remaining -= chunk;
			continue;
		}
		if ( read_block(blk_id, block_buf) != 0 )
		{
			kfree(block_buf);
			return EXT2_ERR_IO;
		}
		size_t chunk = g_block_size - off_in_block;
		if ( chunk > remaining )
			chunk = remaining;
		kmemcpy(dst, block_buf + off_in_block, chunk);
		dst += chunk;
		file->pos += (uint32_t) chunk;
		remaining -= chunk;
	}
	kfree(block_buf);
	return (int) len;
}
