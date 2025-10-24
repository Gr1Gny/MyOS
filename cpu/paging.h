#ifndef PAGING_H
#define PAGING_H

#include "types.h"

#define PAGE_PRESENT   0x1
#define PAGE_WRITABLE  0x2
#define PAGE_USER      0x4

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

#endif