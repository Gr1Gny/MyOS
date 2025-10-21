#include "../cpu/isr.h"
#include "../drivers/screen.h"
#include "../drivers/keyboard.h"
#include "../libc/string.h"
#include "kernel.h"
#include "shell.h"

/* Command history */
static char history[HISTORY_SIZE][256];
static int history_count = 0;
static int history_index = 0;
static int current_history_pos = 0;

/* Prompt color */
static char prompt_color = WHITE_ON_BLACK;

void main() {
    isr_install();
    irq_install();

    clear_screen();
    kprint_color("MyOs> ", prompt_color);
}

void add_to_history(char *cmd) {
    if (strlen(cmd) == 0) return;
    
    strcpy(history[history_index], cmd);
    history_index = (history_index + 1) % HISTORY_SIZE;
    if (history_count < HISTORY_SIZE) history_count++;
    current_history_pos = history_count;
}

char* get_history(int offset) {
    if (history_count == 0) return NULL;
    
    int pos = current_history_pos + offset;
    if (pos < 0) pos = 0;
    if (pos >= history_count) {
        current_history_pos = history_count;
        return NULL;
    }
    
    current_history_pos = pos;
    int actual_index = (history_index - history_count + pos) % HISTORY_SIZE;
    if (actual_index < 0) actual_index += HISTORY_SIZE;
    return history[actual_index];
}

void replace_input_line(char *new_input) {
    /* Clear current line by backspacing */
    int len = strlen(key_buffer);
    for (int i = 0; i < len; i++) {
        kprint_backspace();
    }
    
    /* Copy and print new input */
    strcpy(key_buffer, new_input);
    kprint(key_buffer);
}

char* tab_complete(char *partial) {
    extern command_t commands[];
    int len = strlen(partial);
    
    if (len == 0) return NULL;
    
    /* Find matching command */
    for (int i = 0; i < NUM_COMMANDS; i++) {
        if (strncmp(partial, commands[i].name, len) == 0) {
            return commands[i].name;
        }
    }
    return NULL;
}

char get_prompt_color() {
    return prompt_color;
}

void set_prompt_color(char color) {
    prompt_color = color;
}

void user_input(char *input) {
    add_to_history(input);
    current_history_pos = history_count;
    command_parser(input);
    kprint_color("MyOs> ", prompt_color);
}