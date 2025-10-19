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
  * [TODO] Prevent backspace from erasing prompt ">"

- [x] **Screen Driver**
  * VGA text mode (80x25)
  * Cursor positioning and scrolling
  * Basic color support
  * Backspace handling
  * [TODO] kprint_color() function for colored output
  * [TODO] kprintf() for formatted printing
  * [TODO] Scrollback buffer

- [x] **Memory Management**
  * kmalloc() function with page alignment support
  * memory_copy() and memory_set() utilities
  * Basic heap starting at a fixed address
  * [TODO] kfree() to free allocated memory
  * [TODO] Memory usage tracking

- [x] **Standard Library (libc)**
  * String functions: strlen, strcmp, append, backspace, reverse
  * Number conversion: int_to_ascii, hex_to_ascii
  * Memory operations
  * Port I/O functions (byte and word operations)

- [x] **Shell/Command Interface**
  * Command parser with argument support
  * Commands: help, clear, echo, exit
  * [TODO] Additional commands: mem, time, uptime, version, reboot
  * [TODO] Command history (up/down arrows)
  * [TODO] Tab completion
  * [TODO] Custom shell colors/prompt