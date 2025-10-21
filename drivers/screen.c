#include "screen.h"
#include "../cpu/ports.h"
#include "../libc/mem.h"

/* Declaration of private functions */
s32 get_cursor_offset();
void set_cursor_offset(s32 offset);
s32 print_char(char c, s32 col, s32 row, char attr);
s32 get_offset(s32 col, s32 row);
s32 get_offset_row(s32 offset);
s32 get_offset_col(s32 offset);

/**
 * Print a message on the specified location
 * If col, row, are negative, we will use the current offset
 */
void kprint_at(char *message, s32 col, s32 row, char attr) {
    s32 offset;
    if (!attr) attr = WHITE_ON_BLACK;

    if (col >= 0 && row >= 0)
        offset = get_offset(col, row);
    else {
        offset = get_cursor_offset();
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }

    s32 i = 0;
    while (message[i] != 0) {
        offset = print_char(message[i++], col, row, attr);
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }
}

void kprint(char *message) {
    kprint_at(message, USE_CURRENT_POS, USE_CURRENT_POS, WHITE_ON_BLACK);
}

void kprint_color(char *message, char attr) {
    if (!attr) attr = WHITE_ON_BLACK;
    kprint_at(message, USE_CURRENT_POS, USE_CURRENT_POS, attr);
}

void kprint_backspace() {
    s32 cur = get_cursor_offset();
    s32 row = get_offset_row(cur);
    s32 col = get_offset_col(cur);
    /* If at screen origin, nothing to delete */
    if (row == 0 && col == 0) return;
    /* If at column 0, move to end of previous line */
    if (col == 0) {
        row -= 1;
        col = MAX_COLS - 1;
    } else {
        col -= 1;
    }
    print_char(CHAR_BACKSPACE, col, row, WHITE_ON_BLACK);
}

void kprint_backspace_color(char attr) {
    s32 cur = get_cursor_offset();
    s32 row = get_offset_row(cur);
    s32 col = get_offset_col(cur);
    if (row == 0 && col == 0) return;
    if (col == 0) {
        row -= 1;
        col = MAX_COLS - 1;
    } else {
        col -= 1;
    }
    print_char(CHAR_BACKSPACE, col, row, attr ? attr : WHITE_ON_BLACK);
}

/**
 * Innermost print function for our kernel, directly accesses the video memory 
 *
 * If 'col' and 'row' are negative, we will print at current cursor location
 * If 'attr' is zero it will use 'white on black' as default
 * Returns the offset of the next character
 * Sets the video cursor to the returned offset
 */
s32 print_char(char c, s32 col, s32 row, char attr) {
    u8 *vidmem = (u8*) VIDEO_ADDRESS;
    if (!attr) attr = WHITE_ON_BLACK;

    /* Error control: print a red 'E' if the coords aren't right */
    if (col >= MAX_COLS || row >= MAX_ROWS) {
        vidmem[BYTES_PER_CHAR * MAX_COLS * MAX_ROWS - BYTES_PER_CHAR] = 'E';
        vidmem[BYTES_PER_CHAR * MAX_COLS * MAX_ROWS - 1] = RED_ON_WHITE;
        return get_offset(col, row);
    }

    s32 offset;
    if (col >= 0 && row >= 0) offset = get_offset(col, row);
    else offset = get_cursor_offset();

    if (c == CHAR_NEWLINE) {
        row = get_offset_row(offset);
        offset = get_offset(0, row + 1);
    } else if (c == CHAR_BACKSPACE) {
        vidmem[offset] = ' ';
        vidmem[offset + 1] = attr;
    } else {
        vidmem[offset] = c;
        vidmem[offset + 1] = attr;
        offset += BYTES_PER_CHAR;
    }

    /* Check if the offset is over screen size and scroll */
    if (offset >= MAX_ROWS * MAX_COLS * BYTES_PER_CHAR) {
        s32 i;
        for (i = 1; i < MAX_ROWS; i++) 
            memory_copy((u8*)(get_offset(0, i) + VIDEO_ADDRESS),
                        (u8*)(get_offset(0, i - 1) + VIDEO_ADDRESS),
                        MAX_COLS * BYTES_PER_CHAR);

        /* Blank last line */
        char *last_line = (char*) (get_offset(0, MAX_ROWS - 1) + (u8*) VIDEO_ADDRESS);
        for (i = 0; i < MAX_COLS * BYTES_PER_CHAR; i++) last_line[i] = 0;

        offset -= BYTES_PER_CHAR * MAX_COLS;
    }

    set_cursor_offset(offset);
    return offset;
}

s32 get_cursor_offset() {
    port_byte_out(REG_SCREEN_CTRL, VGA_CURSOR_HIGH);
    s32 offset = port_byte_in(REG_SCREEN_DATA) << 8; /* High byte: << 8 */
    port_byte_out(REG_SCREEN_CTRL, VGA_CURSOR_LOW);
    offset += port_byte_in(REG_SCREEN_DATA);
    return offset * BYTES_PER_CHAR;
}

void set_cursor_offset(s32 offset) {
    /* Similar to get_cursor_offset, but instead of reading we write data */
    offset /= BYTES_PER_CHAR;
    port_byte_out(REG_SCREEN_CTRL, VGA_CURSOR_HIGH);
    port_byte_out(REG_SCREEN_DATA, high_8(offset));
    port_byte_out(REG_SCREEN_CTRL, VGA_CURSOR_LOW);
    port_byte_out(REG_SCREEN_DATA, low_8(offset));
}

void clear_screen() {
    s32 screen_size = MAX_COLS * MAX_ROWS;
    s32 i;
    u8 *screen = (u8*) VIDEO_ADDRESS;

    for (i = 0; i < screen_size; i++) {
        screen[i * BYTES_PER_CHAR] = ' ';
        screen[i * BYTES_PER_CHAR + 1] = WHITE_ON_BLACK;
    }
    set_cursor_offset(get_offset(0, 0));
}

/* Offset calculation helpers */
s32 get_offset(s32 col, s32 row) { 
    return BYTES_PER_CHAR * (row * MAX_COLS + col); 
}

s32 get_offset_row(s32 offset) { 
    return offset / (BYTES_PER_CHAR * MAX_COLS); 
}

s32 get_offset_col(s32 offset) { 
    return (offset / BYTES_PER_CHAR) % MAX_COLS; 
}
