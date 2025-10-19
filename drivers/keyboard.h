#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../cpu/types.h"

/* Keyboard hardware constants */
#define KEYBOARD_DATA_PORT 0x60      /* PS/2 keyboard data port */
#define KEYBOARD_STATUS_PORT 0x64    /* PS/2 keyboard status port */

/* Keyboard buffer size */
#define KEY_BUFFER_SIZE 256

void init_keyboard();

#endif
