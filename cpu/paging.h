#ifndef PAGING_H
#define PAGING_H

#include "types.h"
#include "isr.h"

#define PAGE_PRESENT   0x1
#define PAGE_WRITABLE  0x2
#define PAGE_USER      0x4

#define FRAME_SIZE     0x1000       /* 4KB per frame */
#define FRAMES_PER_BYTE 8           /* 8 frames tracked per byte in bitmap */
#define MEMORY_END     0x1000000    /* 16MB of RAM (adjustable) */
#define TOTAL_FRAMES   (MEMORY_END / FRAME_SIZE)
#define BITMAP_SIZE    (TOTAL_FRAMES / FRAMES_PER_BYTE)

typedef u32 page_entry_t;

typedef struct {
    page_entry_t entries[1024];
} page_table_t;

typedef struct {
    page_entry_t entries[1024];
} page_directory_t;

/* Global paging structures */
extern page_directory_t* kernel_directory;

void init_paging();
void enable_paging();
void page_fault_handler(registers_t regs);

/* Frame allocator functions */
void init_frame_allocator();
u32 alloc_frame();
void free_frame(u32 frame_addr);
u32 get_free_frame_count();
u32 get_used_frame_count();

#endif