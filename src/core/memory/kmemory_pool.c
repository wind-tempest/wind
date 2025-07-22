/* kmemory_pool.c */

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

#include "dlog.h"
#include "kmemory.h"
#include "kstdio.h"
#include "kstring.h"

/* Memory pool management */
memory_pool_t *
    pool_create (ksize_t block_size, ksize_t num_blocks) {
	if ( block_size == 0 || num_blocks == 0 ) {
		return KNULL;
	}

	/* Allocate pool structure */
	memory_pool_t *pool = kmalloc(sizeof(memory_pool_t));
	if ( !pool ) {
		return KNULL;
	}

	/* Calculate total size needed */
	ksize_t total_size = num_blocks * block_size;

	/* Allocate the actual pool memory */
	pool->pool_start = kmalloc(total_size);
	if ( !pool->pool_start ) {
		kfree(pool);
		return KNULL;
	}

	/* Allocate free list */
	pool->free_list = kmalloc(num_blocks * sizeof(void *));
	if ( !pool->free_list ) {
		kfree(pool->pool_start);
		kfree(pool);
		return KNULL;
	}

	/* Initialize pool */
	pool->block_size   = block_size;
	pool->total_blocks = num_blocks;
	pool->free_blocks  = num_blocks;

	/* Initialize free list */
	for ( ksize_t i = 0; i < num_blocks; i++ ) {
		pool->free_list[i] = (kuint8_t *) pool->pool_start + i * block_size;
	}

	return pool;
}

void *
    pool_alloc (memory_pool_t *pool) {
	if ( !pool || pool->free_blocks == 0 ) {
		return KNULL;
	}

	/* Get next free block */
	void *block = pool->free_list[pool->free_blocks - 1];
	pool->free_blocks--;

	return block;
}

void
    pool_free (memory_pool_t *pool, void *ptr) {
	if ( !pool || !ptr ) {
		return;
	}

	/* Check if pointer is within pool bounds */
	if ( ptr < pool->pool_start
	     || (kuintptr_t) ptr
		    >= (kuintptr_t) pool->pool_start + pool->total_blocks * pool->block_size ) {
		return; /* Invalid pointer */
	}

	/* Check if pointer is aligned to block size */
	kuintptr_t offset = (kuintptr_t) ptr - (kuintptr_t) pool->pool_start;
	if ( offset % pool->block_size != 0 ) {
		return; /* Misaligned pointer */
	}

	/* Check if block is already free */
	for ( ksize_t i = 0; i < pool->free_blocks; i++ ) {
		if ( pool->free_list[i] == ptr ) {
			return; /* Already free */
		}
	}

	/* Add to free list */
	if ( pool->free_blocks < pool->total_blocks ) {
		pool->free_list[pool->free_blocks] = ptr;
		pool->free_blocks++;
	}
}

void
    pool_destroy (memory_pool_t *pool) {
	if ( !pool ) {
		return;
	}

	if ( pool->pool_start ) {
		kfree(pool->pool_start);
	}
	if ( pool->free_list ) {
		kfree(pool->free_list);
	}
	kfree(pool);
}

/* Predefined memory pools for common sizes */
static memory_pool_t *small_pool  = KNULL; /* 16 bytes */
static memory_pool_t *medium_pool = KNULL; /* 64 bytes */
static memory_pool_t *large_pool  = KNULL; /* 256 bytes */

void
    init_memory_pools (void) {
	/* Create pools for common allocation sizes */
	small_pool  = pool_create(16, 1024); /* 16KB total */
	medium_pool = pool_create(64, 512);  /* 32KB total */
	large_pool  = pool_create(256, 128); /* 32KB total */

	if ( !small_pool || !medium_pool || !large_pool ) {
		kwarn("Failed to create some memory pools", KNULL);
	}
}

void *
    pool_alloc_small (void) {
	return pool_alloc(small_pool);
}

void *
    pool_alloc_medium (void) {
	return pool_alloc(medium_pool);
}

void *
    pool_alloc_large (void) {
	return pool_alloc(large_pool);
}

void
    pool_free_small (void *ptr) {
	pool_free(small_pool, ptr);
}

void
    pool_free_medium (void *ptr) {
	pool_free(medium_pool, ptr);
}

void
    pool_free_large (void *ptr) {
	pool_free(large_pool, ptr);
}

/* Smart allocation that chooses the best pool */
void *
    smart_alloc (ksize_t size) {
	if ( size <= 16 ) {
		return pool_alloc_small();
	} else if ( size <= 64 ) {
		return pool_alloc_medium();
	} else if ( size <= 256 ) {
		return pool_alloc_large();
	} else {
		return kmalloc(size);
	}
}

void
    smart_free (void *ptr, ksize_t size) {
	if ( size <= 16 ) {
		pool_free_small(ptr);
	} else if ( size <= 64 ) {
		pool_free_medium(ptr);
	} else if ( size <= 256 ) {
		pool_free_large(ptr);
	} else {
		kfree(ptr);
	}
}
