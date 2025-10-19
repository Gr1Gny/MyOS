# --- config ---
CROSS   ?= i686-elf-
CC      := $(CROSS)gcc
LD      := $(CROSS)ld
OBJCOPY := $(CROSS)objcopy
AS      := nasm

CFLAGS  := -g -O2 -ffreestanding -fno-builtin -Wall -Wextra
LDFLAGS := -nostdlib -Ttext 0x1000 -e _start

# Sources & headers
C_SOURCES := $(wildcard kernel/*.c drivers/*.c cpu/*.c libc/*.c)
HEADERS   := $(wildcard kernel/*.h drivers/*.h cpu/*.h libc/*.h)

# Objects: all C objects + the ASM ISR stub
OBJ       := $(C_SOURCES:.c=.o) cpu/interrupt.o

# --- default target ---
os-image.bin: boot/bootsect.bin kernel.bin
	cat $^ > $@

# Flat binary via objcopy keeps symbols in kernel.elf for debugging
kernel.bin: kernel.elf
	$(OBJCOPY) -O binary $< $@

kernel.elf: boot/kernel_entry.o $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $^

# Run & debug
run: os-image.bin
	qemu-system-i386 -fda $<

debug: os-image.bin kernel.elf
	qemu-system-i386 -s -fda os-image.bin -d guest_errors,int &
	$(CROSS)gdb -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

# --- pattern rules ---
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.asm
	$(AS) -f elf $< -o $@

%.bin: %.asm
	$(AS) -f bin $< -o $@

.PHONY: clean run debug
clean:
	rm -f *.bin *.dis *.o os-image.bin *.elf
	rm -f kernel/*.o boot/*.bin drivers/*.o boot/*.o cpu/*.o libc/*.o
