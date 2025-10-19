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

### KERNEL - COMPLETED

- [x] **Boot System**
  * Custom bootloader that loads kernel from disk
  * Real mode to Protected mode (32-bit) transition
  * GDT (Global Descriptor Table) setup

- [x] **Interrupt Handling**
  * IDT (Interrupt Descriptor Table) fully configured
  * All 32 CPU exception handlers (ISRs 0-31)
  * IRQ handlers for hardware interrupts (IRQ0-IRQ15)
  * PIC (Programmable Interrupt Controller) remapping to avoid conflicts

- [x] **Hardware Drivers**
  * Timer Driver: PIT configured at 50Hz
  * Keyboard Driver: PS/2 keyboard with scancode translation
  * Screen Driver: VGA text mode (80x25) with cursor positioning, scrolling, colors, backspace

- [x] **Memory Management (Basic)**
  * kmalloc() function with page alignment support
  * memory_copy() and memory_set() utilities
  * Basic heap starting at a fixed address
  * [TODO] free()

- [x] **Standard Library (libc)**
  * String functions: strlen, strcmp, append, backspace, reverse
  * Number conversion: int_to_ascii, hex_to_ascii
  * Memory operations

- [x] **I/O System**
  * Port I/O functions (byte and word operations)
  * Interrupt-driven keyboard input
  * Text output to VGA display
  * Support lower case text and upper case

- [x] **Shell/Command Interface**
  * Command parser with argument support
  * Commands: HELP, CLEAR, ECHO, EXIT, [TODO] MEMORY, TIME
  * [TODO] Custom shell colors
  * [TODO] Shell command history
  * [TODO] Tabulations to propose commands / args