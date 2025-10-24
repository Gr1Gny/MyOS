#include "paging.h"
#include "../libc/mem.h"
#include "../libc/string.h"
#include "../drivers/screen.h"

page_directory_t* kernel_directory = 0;

void init_paging() {
    register_interrupt_handler(14, page_fault_handler);

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

void page_fault_handler(registers_t regs) {
    u32 faulting_address;
    
    /* Read CR2 to get the faulting address */
    __asm__ __volatile__("mov %%cr2, %0" : "=r"(faulting_address));
    
    /* Decode error code */
    int present = regs.err_code & 0x1;    /* Page not present */
    int rw = regs.err_code & 0x2;         /* Write operation? */
    int us = regs.err_code & 0x4;         /* User mode? */
    int reserved = regs.err_code & 0x8;   /* Reserved bits overwritten? */
    int id = regs.err_code & 0x10;        /* Instruction fetch? */
    
    kprint_color("Page Fault! (", RED_ON_BLACK);
    
    if (!present) kprint_color("page not present ", RED_ON_BLACK);
    if (rw) kprint_color("write ", RED_ON_BLACK);
    else kprint_color("read ", RED_ON_BLACK);
    if (us) kprint_color("user-mode ", RED_ON_BLACK);
    else kprint_color("supervisor-mode ", RED_ON_BLACK);
    if (reserved) kprint_color("reserved ", RED_ON_BLACK);
    if (id) kprint_color("instruction-fetch ", RED_ON_BLACK);
    
    kprint_color(") at 0x", RED_ON_BLACK);
    char addr_str[16];
    hex_to_ascii(faulting_address, addr_str);
    kprint_color(addr_str, RED_ON_BLACK);
    kprint_color("\n", RED_ON_BLACK);
    
    /* Halt the system */
    kprint_color("System halted.\n", RED_ON_BLACK);
    __asm__ __volatile__("cli; hlt");
}