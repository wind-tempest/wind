/* memory_pool.c */

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

#include "dlog.h"
#include "kstdio.h"
#include "kstring.h"
#include "memory.h"

/* Memory pool management */
memory_pool_t *
    pool_create (size_t block_size, size_t num_blocks)
{
	if ( block_size == 0 || num_blocks == 0 )
	{
		return NULL;
	}

	/* Allocate pool structure */
	memory_pool_t *pool = kmalloc(sizeof(memory_pool_t));
	if ( !pool )
	{
		return NULL;
	}

	/* Calculate total size needed */
	size_t total_size = num_blocks * block_size;

	/* Allocate the actual pool memory */
	pool->pool_start = kmalloc(total_size);
	if ( !pool->pool_start )
	{
		kfree(pool);
		return NULL;
	}

	/* Allocate free list */
	pool->free_list = kmalloc(num_blocks * sizeof(void *));
	if ( !pool->free_list )
	{
		kfree(pool->pool_start);
		kfree(pool);
		return NULL;
	}

	/* Initialize pool */
	pool->block_size   = block_size;
	pool->total_blocks = num_blocks;
	pool->free_blocks  = num_blocks;

	/* Initialize free list */
	for ( size_t i = 0; i < num_blocks; i++ )
	{
		pool->free_list[i] = (uint8_t *) pool->pool_start + i * block_size;
	}

	return pool;
}

void *
    pool_alloc (memory_pool_t *pool)
{
	if ( !pool || pool->free_blocks == 0 )
	{
		return NULL;
	}

	/* Get next free block */
	void *block = pool->free_list[pool->free_blocks - 1];
	pool->free_blocks--;

	return block;
}

void
    pool_free (memory_pool_t *pool, void *ptr)
{
	if ( !pool || !ptr )
	{
		return;
	}

	/* Check if pointer is within pool bounds */
	if ( ptr < pool->pool_start
	     || (uintptr_t) ptr
		    >= (uintptr_t) pool->pool_start + pool->total_blocks * pool->block_size )
	{
		return; /* Invalid pointer */
	}

	/* Check if pointer is aligned to block size */
	uintptr_t offset = (uintptr_t) ptr - (uintptr_t) pool->pool_start;
	if ( offset % pool->block_size != 0 )
	{
		return; /* Misaligned pointer */
	}

	/* Check if block is already free */
	for ( size_t i = 0; i < pool->free_blocks; i++ )
	{
		if ( pool->free_list[i] == ptr )
		{
			return; /* Already free */
		}
	}

	/* Add to free list */
	if ( pool->free_blocks < pool->total_blocks )
	{
		pool->free_list[pool->free_blocks] = ptr;
		pool->free_blocks++;
	}
}

void
    pool_destroy (memory_pool_t *pool)
{
	if ( !pool )
	{
		return;
	}

	if ( pool->pool_start )
	{
		kfree(pool->pool_start);
	}
	if ( pool->free_list )
	{
		kfree(pool->free_list);
	}
	kfree(pool);
}

/* Predefined memory pools for common sizes */
static memory_pool_t *small_pool  = NULL; /* 16 bytes */
static memory_pool_t *medium_pool = NULL; /* 64 bytes */
static memory_pool_t *large_pool  = NULL; /* 256 bytes */

void
    init_memory_pools (void)
{
	/* Create pools for common allocation sizes */
	small_pool  = pool_create(16, 1024); /* 16KB total */
	medium_pool = pool_create(64, 512);  /* 32KB total */
	large_pool  = pool_create(256, 128); /* 32KB total */

	if ( !small_pool || !medium_pool || !large_pool )
	{
		kwarn("Failed to create some memory pools", NULL);
	}
}

void *
    pool_alloc_small (void)
{
	return pool_alloc(small_pool);
}

void *
    pool_alloc_medium (void)
{
	return pool_alloc(medium_pool);
}

void *
    pool_alloc_large (void)
{
	return pool_alloc(large_pool);
}

void
    pool_free_small (void *ptr)
{
	pool_free(small_pool, ptr);
}

void
    pool_free_medium (void *ptr)
{
	pool_free(medium_pool, ptr);
}

void
    pool_free_large (void *ptr)
{
	pool_free(large_pool, ptr);
}

/* Smart allocation that chooses the best pool */
void *
    smart_alloc (size_t size)
{
	if ( size <= 16 )
	{
		return pool_alloc_small();
	}
	else if ( size <= 64 )
	{
		return pool_alloc_medium();
	}
	else if ( size <= 256 )
	{
		return pool_alloc_large();
	}
	else
	{
		return kmalloc(size);
	}
}

void
    smart_free (void *ptr, size_t size)
{
	if ( size <= 16 )
	{
		pool_free_small(ptr);
	}
	else if ( size <= 64 )
	{
		pool_free_medium(ptr);
	}
	else if ( size <= 256 )
	{
		pool_free_large(ptr);
	}
	else
	{
		kfree(ptr);
	}
}
