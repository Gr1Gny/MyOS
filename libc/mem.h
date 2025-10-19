#ifndef MEM_H
#define MEM_H

#include "../cpu/types.h"

/* Memory alignment constants */
#define PAGE_SIZE 0x1000           /* 4KB page size */
#define PAGE_ALIGN_MASK 0xFFFFF000 /* Mask to align down to page boundary */
#define PAGE_OFFSET_MASK 0xFFF     /* Mask to get offset within page */

/* Initial free memory address (64KB - after kernel and stack) */
#define KMALLOC_START 0x10000

void memory_copy(u8 *source, u8 *dest, s32 nbytes);
void memory_set(u8 *dest, u8 val, u32 len);

u32 kmalloc(u32 size, u8 align, u32 *phys_addr);
// Todo add a free function

#endif