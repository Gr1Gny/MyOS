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

u32 free_mem_addr = KMALLOC_START;

u32 kmalloc(u32 size, u8 align, u32 *phys_addr) {
    /* If alignment is requested and address is not page-aligned */
    if (align && (free_mem_addr & PAGE_OFFSET_MASK)) {
        free_mem_addr &= PAGE_ALIGN_MASK;
        free_mem_addr += PAGE_SIZE;
    }
    if (phys_addr) *phys_addr = free_mem_addr;

    u32 ret = free_mem_addr;
    free_mem_addr += size;
    return ret;
}
