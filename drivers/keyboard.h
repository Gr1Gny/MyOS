#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../cpu/types.h"

/* Keyboard hardware constants */
#define KEYBOARD_DATA_PORT 0x60      /* PS/2 keyboard data port */
#define KEYBOARD_STATUS_PORT 0x64    /* PS/2 keyboard status port */

/* Keyboard buffer size */
#define KEY_BUFFER_SIZE 256

/* Global key buffer */
extern char key_buffer[KEY_BUFFER_SIZE];

/* Scancode constants */
#define SC_BACKSPACE 0x0E
#define SC_TAB 0x0F
#define SC_ENTER 0x1C
#define SC_LSHIFT 0x2A
#define SC_RSHIFT 0x36
#define SC_LSHIFT_RELEASE 0xAA
#define SC_RSHIFT_RELEASE 0xB6
#define SC_UP_ARROW 0x48
#define SC_DOWN_ARROW 0x50
#define SC_MAX 57

void init_keyboard();

#endif
