#include "keyboard.h"
#include "../cpu/ports.h"
#include "../cpu/isr.h"
#include "screen.h"
#include "../libc/string.h"
#include "../libc/function.h"
#include "../kernel/kernel.h"

/* Shift key state */
static u8 shift_pressed = 0;

static char key_buffer[KEY_BUFFER_SIZE];

const char *sc_name[] = { "ERROR", "Esc", "1", "2", "3", "4", "5", "6", 
    "7", "8", "9", "0", "-", "=", "Backspace", "Tab", "Q", "W", "E", 
        "R", "T", "Y", "U", "I", "O", "P", "[", "]", "Enter", "Lctrl", 
        "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`", 
        "LShift", "\\", "Z", "X", "C", "V", "B", "N", "M", ",", ".", 
        "/", "RShift", "Keypad *", "LAlt", "Spacebar"};

/* Lowercase characters */
const char sc_ascii_lower[] = { 
    '?', '?', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 
    '?', '?', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 
    '?', '?', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 
    '?', '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', '?', '?', 
    '?', ' '
};

/* Uppercase characters */
const char sc_ascii_upper[] = { 
    '?', '?', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 
    '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 
    '?', '?', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 
    '?', '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', '?', '?', 
    '?', ' '
};

static void keyboard_callback(registers_t regs) {
    u8 scancode = port_byte_in(KEYBOARD_DATA_PORT);
    
    /* Handle Shift key press */
    if (scancode == SC_LSHIFT || scancode == SC_RSHIFT) {
        shift_pressed = 1;
        UNUSED(regs);
        return;
    }
    
    /* Handle Shift key release */
    if (scancode == SC_LSHIFT_RELEASE || scancode == SC_RSHIFT_RELEASE) {
        shift_pressed = 0;
        UNUSED(regs);
        return;
    }
    
    if (scancode > SC_MAX) {
        UNUSED(regs);
        return;
    }
    
    if (scancode == SC_BACKSPACE) {
        backspace(key_buffer);
        kprint_backspace();
    } else if (scancode == SC_ENTER) {
        kprint("\n");
        user_input(key_buffer);
        key_buffer[0] = '\0';
    } else {
        char letter;
        if (shift_pressed) {
            letter = sc_ascii_upper[(s32)scancode];
        } else {
            letter = sc_ascii_lower[(s32)scancode];
        }
        
        /* Print and append to buffer */
        char str[2] = {letter, '\0'};
        append(key_buffer, letter);
        kprint(str);
    }
    UNUSED(regs);
}

void init_keyboard() {
   register_interrupt_handler(IRQ1, keyboard_callback); 
}
