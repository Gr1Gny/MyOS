#ifndef SCREEN_H
#define SCREEN_H

#include "../cpu/types.h"

/* VGA text mode constants */
#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define BYTES_PER_CHAR 2  /* Each character cell = 1 byte char + 1 byte attribute */

/* VGA color attributes */
/* Format: 0xBF where B = background color (4 bits), F = foreground color (4 bits) */
/* Color codes: 0=Black, 1=Blue, 2=Green, 3=Cyan, 4=Red, 5=Magenta, 6=Brown, 7=LightGray */
/*              8=DarkGray, 9=LightBlue, A=LightGreen, B=LightCyan, C=LightRed, D=LightMagenta, E=Yellow, F=White */

/* Colors on black background */
#define WHITE_ON_BLACK 0x0f
#define GREEN_ON_BLACK 0x02
#define RED_ON_BLACK 0x04
#define BLUE_ON_BLACK 0x01
#define CYAN_ON_BLACK 0x03
#define MAGENTA_ON_BLACK 0x05
#define YELLOW_ON_BLACK 0x0e
#define LIGHT_GREEN_ON_BLACK 0x0a
#define LIGHT_BLUE_ON_BLACK 0x09
#define LIGHT_RED_ON_BLACK 0x0c
#define LIGHT_CYAN_ON_BLACK 0x0b
#define LIGHT_MAGENTA_ON_BLACK 0x0d

/* Other useful combinations */
#define RED_ON_WHITE 0xf4
#define BLACK_ON_WHITE 0xf0
#define GREEN_ON_BLUE 0x12
#define YELLOW_ON_BLUE 0x1e

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
void kprint_at(char *message, s32 col, s32 row, char attr);
void kprint(char *message);
void kprint_color(char *message, char attr);
void kprint_backspace();
void kprint_backspace_color(char attr);

#endif
