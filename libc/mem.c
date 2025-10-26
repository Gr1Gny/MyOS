#include "mem.h"

void memory_copy(u8 *source, u8 *dest, s32 nbytes) {
    s32 i;
    for (i = 0; i < nbytes; i++) {
        *(dest + i) = *(source + i);
    }
}

void memory_set(u8 *dest, u8 val, u32 len) {
    u8 *temp = (u8 *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}

/* Block header for heap allocations */
typedef struct heap_block {
    u32 size;
    u32 is_free;
    struct heap_block *next;
} heap_block_t;

#define BLOCK_HEADER_SIZE sizeof(heap_block_t)

/* Heap state */
u32 free_mem_addr = KMALLOC_START;
static heap_block_t *free_list = 0;  /* Linked list of free blocks */
static u32 total_allocated = 0;
static u32 total_freed = 0;

/* Find free block that fits size (first-fit) */
static heap_block_t* find_free_block(u32 size) {
    heap_block_t *current = free_list;
    while (current) {
        if (current->is_free && current->size >= size) {
            return current;
        }
        current = current->next;
    }
    return 0;
}

/* allocate memory on the heap */
u32 kmalloc(u32 size, u8 align, u32 *phys_addr) {
    if (align) {
        if (free_mem_addr & PAGE_OFFSET_MASK) {
            free_mem_addr &= PAGE_ALIGN_MASK;
            free_mem_addr += PAGE_SIZE;
        }
        u32 ret = free_mem_addr;
        free_mem_addr += size;
        total_allocated += size;
        if (phys_addr) *phys_addr = ret;
        return ret;
    }

    heap_block_t *block = find_free_block(size);
    if (block) {
        block->is_free = 0;
        u32 addr = (u32)block + BLOCK_HEADER_SIZE;
        if (phys_addr) *phys_addr = addr;
        return addr;
    }

    heap_block_t *new_block = (heap_block_t*)free_mem_addr;
    new_block->size = size;
    new_block->is_free = 0;
    new_block->next = free_list;
    free_list = new_block;

    u32 ret = free_mem_addr + BLOCK_HEADER_SIZE;
    free_mem_addr += BLOCK_HEADER_SIZE + size;
    total_allocated += size;

    if (phys_addr) *phys_addr = ret;
    return ret;
}

/* free memory on the heap */
void kfree(void *ptr) {
    if (!ptr) return;
    
    heap_block_t *block = (heap_block_t*)((u32)ptr - BLOCK_HEADER_SIZE);
    
    if (!block->is_free) {
        block->is_free = 1;
        total_freed += block->size;
    }
}

/* get heap statistics */
void get_heap_stats(u32 *total, u32 *used, u32 *free_mem) {
    *total = free_mem_addr - KMALLOC_START;
    *used = total_allocated - total_freed;
    *free_mem = *total - *used;
}
