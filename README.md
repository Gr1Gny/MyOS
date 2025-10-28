# MyOS - 32-bit Kernel Development

A simple operating system kernel written from scratch in C and x86 Assembly.

## Build & Run

```bash
make          # Build the OS
make run      # Run in QEMU
make debug    # Debug with GDB
make clean    # Clean build files
```

## ✅ Implementation Status

- [x] **Boot System**
  * Custom bootloader that loads kernel from disk
  * Real mode to Protected mode (32-bit) transition
  * GDT (Global Descriptor Table) setup

- [x] **Interrupt Handling**
  * IDT (Interrupt Descriptor Table) fully configured
  * All 32 CPU exception handlers (ISRs 0-31)
  * IRQ handlers for hardware interrupts (IRQ0-IRQ15)
  * PIC (Programmable Interrupt Controller) remapping to avoid conflicts

- [x] **Timer Driver**
  * PIT configured at 50Hz
  * Tick counter for system uptime

- [x] **Keyboard Driver**
  * PS/2 keyboard with scancode translation
  * Lowercase/uppercase support with Shift key
  * [TODO] Caps Lock support
  * [TODO] Ctrl key combinations (Ctrl+C, Ctrl+L)
  * [TODO] Arrow key navigation
  * Prevent backspace from erasing prompt ">"

- [x] **Screen Driver**
  * VGA text mode (80x25)
  * Cursor positioning and scrolling
  * Basic color support
  * Backspace handling
  * kprint_color() function for colored output
  * kprintf() for formatted printing

- [x] **Memory Management**
  * kmalloc() function with page alignment support
  * memory_copy() and memory_set() utilities
  * Basic heap starting at a fixed address
  * Add dynamic heap management
  * [TODO] CR3 is expecting page aligned address so I removed heap block header for now (it is a problem as they can not be free anymore), solutions :
  - add a padding before the page to put the header
  - change structure and keep an address table on the side
  - no alignment in mem.c, do it directly in the frame allocator
  * [TODO] Overwrite size while reusing memory in the heap to avoid memory loss

- [x] **Standard Library (libc)**
  * String functions: strlen, strcmp, append, backspace, reverse
  * Number conversion: int_to_ascii, hex_to_ascii
  * Memory operations
  * Port I/O functions (byte and word operations)

- [x] **Shell/Command Interface**
  * Command parser with argument support
  * Commands: help, clear, echo, mem, exit
  * [TODO] Additional commands: time, uptime, version, reboot
  * Command history (up/down arrows) - Use arrow keys to navigate through command history
  * Tab completion - Press Tab to autocomplete commands
  * Custom typing/output color - Use 'prompt <color>' command

- [x] **Paging & Virtual Memory**
  * Set up page tables and page directory
  * Enable paging (CR0 register)
  * Implement page fault handler (ISR 14)
  * Virtual memory mapping (identity mapping first)
  * Physical page frame allocator (bitmap)
  * Functions: alloc_frame(), free_frame(), get stats

- [] **Process/Task Management**
  * [TODO] Store CPU state (registers, stack, ...)
  * [TODO] Save/restore CPU state
  * [TODO] Implement a simple round-robin scheduler
  * [TODO] Add concurrent execution

- [] **File system**

- [] **User mode**