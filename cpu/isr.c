#include "isr.h"
#include "idt.h"
#include "../drivers/screen.h"
#include "../drivers/keyboard.h"
#include "../libc/string.h"
#include "timer.h"
#include "ports.h"

isr_t interrupt_handlers[MAX_INTERRUPTS];

void isr_install() {
    set_idt_gate(0, (u32)isr0);
    set_idt_gate(1, (u32)isr1);
    set_idt_gate(2, (u32)isr2);
    set_idt_gate(3, (u32)isr3);
    set_idt_gate(4, (u32)isr4);
    set_idt_gate(5, (u32)isr5);
    set_idt_gate(6, (u32)isr6);
    set_idt_gate(7, (u32)isr7);
    set_idt_gate(8, (u32)isr8);
    set_idt_gate(9, (u32)isr9);
    set_idt_gate(10, (u32)isr10);
    set_idt_gate(11, (u32)isr11);
    set_idt_gate(12, (u32)isr12);
    set_idt_gate(13, (u32)isr13);
    set_idt_gate(14, (u32)isr14);
    set_idt_gate(15, (u32)isr15);
    set_idt_gate(16, (u32)isr16);
    set_idt_gate(17, (u32)isr17);
    set_idt_gate(18, (u32)isr18);
    set_idt_gate(19, (u32)isr19);
    set_idt_gate(20, (u32)isr20);
    set_idt_gate(21, (u32)isr21);
    set_idt_gate(22, (u32)isr22);
    set_idt_gate(23, (u32)isr23);
    set_idt_gate(24, (u32)isr24);
    set_idt_gate(25, (u32)isr25);
    set_idt_gate(26, (u32)isr26);
    set_idt_gate(27, (u32)isr27);
    set_idt_gate(28, (u32)isr28);
    set_idt_gate(29, (u32)isr29);
    set_idt_gate(30, (u32)isr30);
    set_idt_gate(31, (u32)isr31);

    /* Remap the PIC (Programmable Interrupt Controller)
     * 
     * PROBLEM: By default, the PIC maps hardware IRQs 0-7 to interrupt vectors 8-15.
     *          But the CPU already uses vectors 0-31 for exceptions (divide by zero, page fault, etc.)
     *          This creates a conflict! For example, IRQ0 (timer) would trigger vector 8, 
     *          which is also the "Double Fault" exception.
     * 
     * SOLUTION: Remap the PIC so hardware IRQs don't overlap with CPU exceptions:
     *           - Master PIC: IRQs 0-7  → vectors 32-39  (IRQ0-IRQ7)
     *           - Slave PIC:  IRQs 8-15 → vectors 40-47  (IRQ8-IRQ15)
     * 
     * The two PICs are connected in a "cascade" - the slave PIC connects to the master's IRQ2.
     */
    
    /* Step 1: Initialize both PICs */
    port_byte_out(PIC_MASTER_COMMAND, PIC_ICW1_INIT);
    port_byte_out(PIC_SLAVE_COMMAND, PIC_ICW1_INIT);
    
    /* Step 2: Set vector offsets (where IRQs start in the interrupt table) */
    port_byte_out(PIC_MASTER_DATA, IRQ0);      /* Master starts at vector 32 */
    port_byte_out(PIC_SLAVE_DATA, IRQ8);       /* Slave starts at vector 40 */
    
    /* Step 3: Configure the cascade (how the PICs are connected) */
    port_byte_out(PIC_MASTER_DATA, PIC_CASCADE_IRQ);  /* Tell master: slave is at IRQ2 */
    port_byte_out(PIC_SLAVE_DATA, PIC_SLAVE_ID);      /* Tell slave: you are slave #2 */
    
    /* Step 4: Set mode (8086 mode) */
    port_byte_out(PIC_MASTER_DATA, PIC_ICW4_8086);
    port_byte_out(PIC_SLAVE_DATA, PIC_ICW4_8086);
    
    /* Step 5: Unmask all IRQs (enable them) */
    port_byte_out(PIC_MASTER_DATA, PIC_UNMASK_ALL);
    port_byte_out(PIC_SLAVE_DATA, PIC_UNMASK_ALL); 

    /* Install the IRQ handlers */
    set_idt_gate(IRQ0, (u32)irq0);
    set_idt_gate(IRQ1, (u32)irq1);
    set_idt_gate(IRQ2, (u32)irq2);
    set_idt_gate(IRQ3, (u32)irq3);
    set_idt_gate(IRQ4, (u32)irq4);
    set_idt_gate(IRQ5, (u32)irq5);
    set_idt_gate(IRQ6, (u32)irq6);
    set_idt_gate(IRQ7, (u32)irq7);
    set_idt_gate(IRQ8, (u32)irq8);
    set_idt_gate(IRQ9, (u32)irq9);
    set_idt_gate(IRQ10, (u32)irq10);
    set_idt_gate(IRQ11, (u32)irq11);
    set_idt_gate(IRQ12, (u32)irq12);
    set_idt_gate(IRQ13, (u32)irq13);
    set_idt_gate(IRQ14, (u32)irq14);
    set_idt_gate(IRQ15, (u32)irq15);

    set_idt(); // Load with ASM
}

/* To print the message which defines every exception */
char *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void isr_handler(registers_t r) {
    kprint("received interrupt: ");
    char s[3];
    int_to_ascii((s32)r.int_no, s);
    kprint(s);
    kprint("\n");
    kprint(exception_messages[r.int_no]);
    kprint("\n");
}

void register_interrupt_handler(u8 n, isr_t handler) {
    interrupt_handlers[n] = handler;
}

void irq_handler(registers_t r) {
    /* After every interrupt we need to send an EOI (End Of Interrupt) to the PICs
     * or they will not send another interrupt again */
    if (r.int_no >= IRQ8) {
        /* If the IRQ came from the slave PIC, send EOI to slave */
        port_byte_out(PIC_SLAVE_COMMAND, PIC_EOI);
    }
    /* Always send EOI to master PIC */
    port_byte_out(PIC_MASTER_COMMAND, PIC_EOI);

    if (interrupt_handlers[r.int_no] != NULL) {
        isr_t handler = interrupt_handlers[r.int_no];
        handler(r);
    }
}

void irq_install() {
    /* Enable interruptions */
    asm volatile("sti");
    /* IRQ0: timer */
    init_timer(50);
    /* IRQ1: keyboard */
    init_keyboard();
}
