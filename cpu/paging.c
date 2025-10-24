#include "paging.h"
#include "../libc/mem.h"
#include "../drivers/screen.h"

page_directory_t* kernel_directory = 0;

void init_paging() {
    u32 phys_dir_addr;
    u32 phys_table_addr;

    /* Allocate page-aligned page directory */
    kernel_directory = (page_directory_t*) kmalloc(sizeof(page_directory_t), 1, &phys_dir_addr);
    memory_set((u8*)kernel_directory, 0, sizeof(page_directory_t));

    /* Allocate page-aligned page table */
    page_table_t* first_table = (page_table_t*) kmalloc(sizeof(page_table_t), 1, &phys_table_addr);
    memory_set((u8*)first_table, 0, sizeof(page_table_t));

    /* Identity map first 4MB (0x00000000 - 0x003FFFFF) */
    for (u32 i = 0; i < 1024; i++) {
        u32 phys = i * 0x1000;
        first_table->entries[i] = (phys & 0xFFFFF000) | PAGE_PRESENT | PAGE_WRITABLE;
    }

    /* Link page table into page directory using physical address */
    kernel_directory->entries[0] = (phys_table_addr & 0xFFFFF000) | PAGE_PRESENT | PAGE_WRITABLE;

    kprint_color("Paging structures initialized\n", GREEN_ON_BLACK);
}

void enable_paging() {
    u32 cr0;
    
    /* Load page directory address into CR3 */
    __asm__ __volatile__("mov %0, %%cr3" :: "r"(kernel_directory));
    
    /* Enable paging by setting bit 31 in CR0 */
    __asm__ __volatile__("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    __asm__ __volatile__("mov %0, %%cr0" :: "r"(cr0));
    
    kprint_color("Paging enabled!\n", GREEN_ON_BLACK);
}