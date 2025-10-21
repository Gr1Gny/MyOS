#include "keyboard.h"
#include "../cpu/ports.h"
#include "../cpu/isr.h"
#include "screen.h"
#include "../libc/string.h"
#include "../libc/function.h"
#include "../kernel/kernel.h"

/* Shift key state */
static u8 shift_pressed = 0;

/* Track extended scancode prefix (0xE0) */
static u8 extended_code = 0;

char key_buffer[KEY_BUFFER_SIZE];

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

/* Safely set current input to new_input by editing in-place */
static void set_input_to(char *new_input) {
    s32 cur_len = strlen(key_buffer);
    s32 new_len = strlen(new_input);
    s32 i = 0;
    while (i < cur_len && i < new_len && key_buffer[i] == new_input[i]) i++;
    /* Remove extra tail from current */
    for (s32 j = cur_len; j > i; j--) {
        backspace(key_buffer);
        kprint_backspace();
    }
    /* Append missing suffix */
    for (s32 j = i; j < new_len; j++) {
        char ch = new_input[j];
        append(key_buffer, ch);
        char out[2] = {ch, '\0'};
        kprint(out);
    }
}

static void keyboard_callback(registers_t regs) {
    u8 scancode = port_byte_in(KEYBOARD_DATA_PORT);

    /* Handle extended scancode prefix */
    if (scancode == 0xE0) {
        extended_code = 1;
        UNUSED(regs);
        return;
    }

    /* If previous byte was 0xE0, handle extended keys (arrows) */
    if (extended_code) {
        extended_code = 0;
        /* Ignore key releases (high bit set) */
        if (scancode & 0x80) {
            UNUSED(regs);
            return;
        }
        if (scancode == SC_UP_ARROW) {
            char *prev_cmd = get_history(-1);
            if (prev_cmd != NULL) set_input_to(prev_cmd);
            UNUSED(regs);
            return;
        }
        if (scancode == SC_DOWN_ARROW) {
            char *next_cmd = get_history(1);
            if (next_cmd != NULL) set_input_to(next_cmd);
            else set_input_to("");
            UNUSED(regs);
            return;
        }
        /* Unhandled extended key */
        UNUSED(regs);
        return;
    }

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

    /* Handle tab (command completion) */
    if (scancode == SC_TAB) {
        char *completed = tab_complete(key_buffer);
        if (completed != NULL) {
            s32 cur_len = strlen(key_buffer);
            s32 i = 0;
            while (completed[cur_len + i] != '\0') {
                char ch = completed[cur_len + i];
                append(key_buffer, ch);
                char out[2] = {ch, '\0'};
                kprint(out);
                i++;
            }
        }
        UNUSED(regs);
        return;
    }

    if (scancode > SC_MAX) {
        UNUSED(regs);
        return;
    }
    
    if (scancode == SC_BACKSPACE) {
        if (strlen(key_buffer) > 0) {
            backspace(key_buffer);
            kprint_backspace();
        }
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
