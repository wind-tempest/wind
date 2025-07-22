/* kmemory.h */

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

#include "kstdbool.h"
#include "kstddef.h"
#include "kstdint.h"

/* Memory constants */
#define PAGE_SIZE	      4096
#define PAGE_SIZE_2MB	      (2 * 1024 * 1024)
#define PAGE_SIZE_1GB	      (1024 * 1024 * 1024)
#define PAGE_MASK	      (PAGE_SIZE - 1)
#define PAGE_ALIGN_UP(addr)   (((addr) + PAGE_MASK) & ~PAGE_MASK)
#define PAGE_ALIGN_DOWN(addr) ((addr) & ~PAGE_MASK)
#define PAGE_INDEX(addr)      ((addr) >> 12)

/* Virtual memory layout */
#define KERNEL_BASE	 0xFFFFFFFF80000000
#define KERNEL_HEAP_BASE 0x01000000	    // 16 MB, identity-mapped in early paging setup
#define KERNEL_HEAP_SIZE (64 * 1024 * 1024) // 64MB
#define USER_SPACE_BASE	 0x0000000000400000
#define USER_SPACE_SIZE	 (0x800000000000 - USER_SPACE_BASE)

/* Page table entry flags */
#define PAGE_PRESENT	   0x001
#define PAGE_WRITABLE	   0x002
#define PAGE_USER	   0x004
#define PAGE_WRITETHROUGH  0x008
#define PAGE_CACHE_DISABLE 0x010
#define PAGE_ACCESSED	   0x020
#define PAGE_DIRTY	   0x040
#define PAGE_HUGE	   0x080
#define PAGE_GLOBAL	   0x100
#define PAGE_NX		   0x8000000000000000

/* Memory allocation flags */
#define MEMORY_ZERO	  0x001
#define MEMORY_USER	  0x002
#define MEMORY_WRITABLE	  0x004
#define MEMORY_EXECUTABLE 0x008

/* Physical memory regions */
typedef enum {
	MEMORY_USABLE		= 1,
	MEMORY_RESERVED		= 2,
	MEMORY_ACPI_RECLAIMABLE = 3,
	MEMORY_ACPI_NVS		= 4,
	MEMORY_BAD		= 5
} memory_type_t;

typedef struct {
	kuint64_t     base_addr;
	kuint64_t     length;
	memory_type_t type;
	kuint32_t     acpi_attributes;
} memory_map_entry_t;

/* Page frame structure */
typedef struct page_frame {
	struct page_frame *next;
	kuint64_t	   physical_addr;
	kuint32_t	   ref_count;
	kbool		   is_free;
} page_frame_t;

/* Virtual memory region */
typedef struct vm_region {
	kuint64_t	  start;
	kuint64_t	  end;
	kuint64_t	  flags;
	struct vm_region *next;
} vm_region_t;

/* Heap block header */
typedef struct heap_block {
	ksize_t		   size;
	kbool		   is_free;
	struct heap_block *next;
	struct heap_block *prev;
} heap_block_t;

/* Memory pool */
typedef struct memory_pool {
	void   *pool_start;
	ksize_t block_size;
	ksize_t total_blocks;
	ksize_t free_blocks;
	void  **free_list;
} memory_pool_t;

/* Memory statistics */
typedef struct {
	kuint64_t total_physical_pages;
	kuint64_t free_physical_pages;
	kuint64_t used_physical_pages;
	kuint64_t total_heap_size;
	kuint64_t free_heap_size;
	kuint64_t used_heap_size;
} memory_stats_t;

/* Page table structures */
typedef kuint64_t pml4e_t;
typedef kuint64_t pml3e_t;
typedef kuint64_t pml2e_t;
typedef kuint64_t pml1e_t;

typedef struct {
	pml4e_t entries[512];
} pml4_t;

typedef struct {
	pml3e_t entries[512];
} pml3_t;

typedef struct {
	pml2e_t entries[512];
} pml2_t;

typedef struct {
	pml1e_t entries[512];
} pml1_t;

/* Function prototypes */

/* Physical memory management */
void
    kmemory_init (void *multiboot_info);
page_frame_t *
    allocate_page_frame (void);
void
    free_page_frame (page_frame_t *frame);
kuint64_t
    get_physical_addr (page_frame_t *frame);
page_frame_t *
    get_page_frame (kuint64_t physical_addr);

/* Virtual memory management */
void
    vm_init (void);
kbool
    vm_map_page (kuint64_t virtual_addr, kuint64_t physical_addr, kuint64_t flags);
kbool
    vm_unmap_page (kuint64_t virtual_addr);
kuint64_t
    vm_get_physical_addr (kuint64_t virtual_addr);
void
    vm_invalidate_page (kuint64_t virtual_addr);
void
    vm_switch_pagetable (pml4_t *new_pml4);

/* Heap management */
void
    heap_init (void);
void *
    kmalloc (ksize_t size);
void *
    kcalloc (ksize_t count, ksize_t size);
void *
    krealloc (void *ptr, ksize_t size);
void
    kfree (void *ptr);
void
    heap_defrag (void);

/* Memory pool management */
memory_pool_t *
    pool_create (ksize_t block_size, ksize_t num_blocks);
void *
    pool_alloc (memory_pool_t *pool);
void
    pool_free (memory_pool_t *pool, void *ptr);
void
    pool_destroy (memory_pool_t *pool);
void
    init_memory_pools (void);
void *
    pool_alloc_small (void);
void *
    pool_alloc_medium (void);
void *
    pool_alloc_large (void);
void
    pool_free_small (void *ptr);
void
    pool_free_medium (void *ptr);
void
    pool_free_large (void *ptr);
void *
    smart_alloc (ksize_t size);
void
    smart_free (void *ptr, ksize_t size);

/* Memory statistics */
memory_stats_t
    memory_get_stats (void);
void
    memory_print_stats (void);

/* Memory protection */
kbool
    memory_protect (kuint64_t addr, ksize_t size, kuint64_t flags);
kbool
    memory_unprotect (kuint64_t addr, ksize_t size);

/* Memory mapping */
void *
    memory_map_physical (kuint64_t physical_addr, ksize_t size, kuint64_t flags);
void
    memory_unmap_physical (void *virtual_addr, ksize_t size);

/* Kernel memory management */
void *
    kernel_alloc (ksize_t size);
void *
    kernel_alloc_aligned (ksize_t size, ksize_t alignment);
void
    kernel_free (void *ptr);

/* User memory management */
void *
    user_alloc (ksize_t size);
void
    user_free (void *ptr);
kbool
    user_copy_to_kernel (void *kernel_dest, const void *user_src, ksize_t size);
kbool
    user_copy_from_kernel (void *user_dest, const void *kernel_src, ksize_t size);
