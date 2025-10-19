#ifndef SCREEN_H
#define SCREEN_H

#include "../cpu/types.h"

/* VGA text mode constants */
#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define BYTES_PER_CHAR 2  /* Each character cell = 1 byte char + 1 byte attribute */

/* VGA color attributes */
#define WHITE_ON_BLACK 0x0f
#define RED_ON_WHITE 0xf4

/* VGA cursor control registers */
#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5
#define VGA_CURSOR_HIGH 14   /* Register for cursor position high byte */
#define VGA_CURSOR_LOW  15   /* Register for cursor position low byte */

/* Special characters */
#define CHAR_BACKSPACE 0x08
#define CHAR_NEWLINE '\n'

/* Value for "use current cursor position" */
#define USE_CURRENT_POS -1

/* Public kernel API */
void clear_screen();
void kprint_at(char *message, s32 col, s32 row);
void kprint(char *message);
void kprint_backspace();

#endif
