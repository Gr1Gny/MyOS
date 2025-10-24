#include "shell.h"
#include "../drivers/screen.h"
#include "../libc/string.h"
#include "kernel.h"
#include "../libc/function.h"

extern command_t commands[];

void help(char *args) {
    for (int i = 0; i < NUM_COMMANDS; i++) {
        kprint_color(commands[i].name, get_input_color());
        kprint_color(" - ", get_input_color());
        kprint_color(commands[i].description, get_input_color());
        kprint_color("\n", get_input_color());
    }
    UNUSED(args);
}

void clear(char *args) {
    clear_screen();
}

void echo(char *args) {
    if (args != NULL) {
        kprint_color(args, get_input_color());
    }
    kprint_color("\n", get_input_color());
    UNUSED(args);
}

void shell_exit(char *args) {
    kprint_color("Halting CPU...\n", get_input_color());
    __asm__ __volatile__("hlt");
    UNUSED(args);
}

void prompt(char *args) {
    if (args == NULL) {
        kprint_color("Usage: prompt <color>\n", get_input_color());
        kprint_color("Colors: green, blue, red, cyan, yellow, magenta, white\n", get_input_color());
        return;
    }
    
    char color = WHITE_ON_BLACK;
    if (strcmp(args, "green") == 0) color = GREEN_ON_BLACK;
    else if (strcmp(args, "blue") == 0) color = BLUE_ON_BLACK;
    else if (strcmp(args, "red") == 0) color = RED_ON_BLACK;
    else if (strcmp(args, "cyan") == 0) color = CYAN_ON_BLACK;
    else if (strcmp(args, "yellow") == 0) color = YELLOW_ON_BLACK;
    else if (strcmp(args, "magenta") == 0) color = MAGENTA_ON_BLACK;
    else if (strcmp(args, "white") == 0) color = WHITE_ON_BLACK;
    else {
        kprint_color("Unknown color. Available: green, blue, red, cyan, yellow, magenta, white\n", get_input_color());
        return;
    }
    
    set_input_color(color);
}

void unknown_command() {
    kprint_color("Unknown command. Type 'help' for available commands.\n", get_input_color());
}

/* Command table - defined AFTER the functions so they exist */
command_t commands[] = {
    {"help", help, "Show this help message"},
    {"clear", clear, "Clear the screen"},
    {"echo", echo, "Print a message"},
    {"prompt", prompt, "Change prompt color"},
    {"exit", shell_exit, "Halt the CPU"}
};

/* Main command parser */
void command_parser(char *input) {
    char *args = NULL;

    /* Parse command and arguments */
    int i = 0;
    while (input[i] != '\0') {
        if (input[i] == ' ') {
            input[i] = '\0'; 
            args = &input[i+1];
            break;
        }
        i++;
    }

    /* Look up command in table */
    for (int i = 0; i < NUM_COMMANDS; i++) {
        if (strcmp(input, commands[i].name) == 0) {
            commands[i].handler(args);
            return;
        }
    }
    
    /* Command not found */
    if (input[0] != '\0') {
        unknown_command();
    }
}