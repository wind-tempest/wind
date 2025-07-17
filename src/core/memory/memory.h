/* memory.h */

/*
 * This file is part of Wind/Tempest
 *
 * Wind/Tempest is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Wind/Tempest is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"

/* Memory constants */
#define PAGE_SIZE             4096
#define PAGE_SIZE_2MB         (2 * 1024 * 1024)
#define PAGE_SIZE_1GB         (1024 * 1024 * 1024)
#define PAGE_MASK             (PAGE_SIZE - 1)
#define PAGE_ALIGN_UP(addr)   (((addr) + PAGE_MASK) & ~PAGE_MASK)
#define PAGE_ALIGN_DOWN(addr) ((addr) & ~PAGE_MASK)
#define PAGE_INDEX(addr)      ((addr) >> 12)

/* Virtual memory layout */
#define KERNEL_BASE      0xFFFFFFFF80000000
#define KERNEL_HEAP_BASE 0x01000000         // 16 MB, identity-mapped in early paging setup
#define KERNEL_HEAP_SIZE (64 * 1024 * 1024) // 64MB
#define USER_SPACE_BASE  0x0000000000400000
#define USER_SPACE_SIZE  (0x800000000000 - USER_SPACE_BASE)

/* Page table entry flags */
#define PAGE_PRESENT       0x001
#define PAGE_WRITABLE      0x002
#define PAGE_USER          0x004
#define PAGE_WRITETHROUGH  0x008
#define PAGE_CACHE_DISABLE 0x010
#define PAGE_ACCESSED      0x020
#define PAGE_DIRTY         0x040
#define PAGE_HUGE          0x080
#define PAGE_GLOBAL        0x100
#define PAGE_NX            0x8000000000000000

/* Memory allocation flags */
#define MEMORY_ZERO       0x001
#define MEMORY_USER       0x002
#define MEMORY_WRITABLE   0x004
#define MEMORY_EXECUTABLE 0x008

/* Physical memory regions */
typedef enum
{
        MEMORY_USABLE           = 1,
        MEMORY_RESERVED         = 2,
        MEMORY_ACPI_RECLAIMABLE = 3,
        MEMORY_ACPI_NVS         = 4,
        MEMORY_BAD              = 5
} memory_type_t;

typedef struct
{
        uint64_t      base_addr;
        uint64_t      length;
        memory_type_t type;
        uint32_t      acpi_attributes;
} memory_map_entry_t;

/* Page frame structure */
typedef struct page_frame
{
        struct page_frame *next;
        uint64_t           physical_addr;
        uint32_t           ref_count;
        bool               is_free;
} page_frame_t;

/* Virtual memory region */
typedef struct vm_region
{
        uint64_t          start;
        uint64_t          end;
        uint64_t          flags;
        struct vm_region *next;
} vm_region_t;

/* Heap block header */
typedef struct heap_block
{
        size_t             size;
        bool               is_free;
        struct heap_block *next;
        struct heap_block *prev;
} heap_block_t;

/* Memory pool */
typedef struct memory_pool
{
        void  *pool_start;
        size_t block_size;
        size_t total_blocks;
        size_t free_blocks;
        void **free_list;
} memory_pool_t;

/* Memory statistics */
typedef struct
{
        uint64_t total_physical_pages;
        uint64_t free_physical_pages;
        uint64_t used_physical_pages;
        uint64_t total_heap_size;
        uint64_t free_heap_size;
        uint64_t used_heap_size;
} memory_stats_t;

/* Page table structures */
typedef uint64_t pml4e_t;
typedef uint64_t pml3e_t;
typedef uint64_t pml2e_t;
typedef uint64_t pml1e_t;

typedef struct
{
        pml4e_t entries[512];
} pml4_t;

typedef struct
{
        pml3e_t entries[512];
} pml3_t;

typedef struct
{
        pml2e_t entries[512];
} pml2_t;

typedef struct
{
        pml1e_t entries[512];
} pml1_t;

/* Function prototypes */

/* Physical memory management */
void
    memory_init (void *multiboot_info);
page_frame_t *
    allocate_page_frame (void);
void
    free_page_frame (page_frame_t *frame);
uint64_t
    get_physical_addr (page_frame_t *frame);
page_frame_t *
    get_page_frame (uint64_t physical_addr);

/* Virtual memory management */
void
    vm_init (void);
bool
    vm_map_page (uint64_t virtual_addr, uint64_t physical_addr, uint64_t flags);
bool
    vm_unmap_page (uint64_t virtual_addr);
uint64_t
    vm_get_physical_addr (uint64_t virtual_addr);
void
    vm_invalidate_page (uint64_t virtual_addr);
void
    vm_switch_pagetable (pml4_t *new_pml4);

/* Heap management */
void
    heap_init (void);
void *
    kmalloc (size_t size);
void *
    kcalloc (size_t count, size_t size);
void *
    krealloc (void *ptr, size_t size);
void
    kfree (void *ptr);
void
    heap_defrag (void);

/* Memory pool management */
memory_pool_t *
    pool_create (size_t block_size, size_t num_blocks);
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
    smart_alloc (size_t size);
void
    smart_free (void *ptr, size_t size);

/* Memory utilities */
void *
    memset (void *dest, int c, size_t n);
void *
    memcpy (void *dest, const void *src, size_t n);
int
    memcmp (const void *s1, const void *s2, size_t n);
void *
    memmove (void *dest, const void *src, size_t n);

/* Memory statistics */
memory_stats_t
    memory_get_stats (void);
void
    memory_print_stats (void);

/* Memory protection */
bool
    memory_protect (uint64_t addr, size_t size, uint64_t flags);
bool
    memory_unprotect (uint64_t addr, size_t size);

/* Memory mapping */
void *
    memory_map_physical (uint64_t physical_addr, size_t size, uint64_t flags);
void
    memory_unmap_physical (void *virtual_addr, size_t size);

/* Kernel memory management */
void *
    kernel_alloc (size_t size);
void *
    kernel_alloc_aligned (size_t size, size_t alignment);
void
    kernel_free (void *ptr);

/* User memory management */
void *
    user_alloc (size_t size);
void
    user_free (void *ptr);
bool
    user_copy_to_kernel (void *kernel_dest, const void *user_src, size_t size);
bool
    user_copy_from_kernel (void *user_dest, const void *kernel_src, size_t size);
