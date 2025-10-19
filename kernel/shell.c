#include "shell.h"
#include "../drivers/screen.h"
#include "../libc/string.h"

extern command_t commands[];

void help(char *args) {
    for (int i = 0; i < NUM_COMMANDS; i++) {
        kprint(commands[i].name);
        kprint(" - ");
        kprint(commands[i].description);
        kprint("\n");
    }
}

void clear(char *args) {
    clear_screen();
}

void echo(char *args) {
    if (args != NULL) {
        kprint(args);
    }
    kprint("\n");
}

void shell_exit(char *args) {
    kprint("Halting CPU...\n");
    __asm__ __volatile__("hlt");
}

void unknown_command() {
    kprint("Unknown command. Type 'HELP' for available commands.\n");
}

/* Command table - defined AFTER the functions so they exist */
command_t commands[] = {
    {"HELP", help, "Show this help message"},
    {"CLEAR", clear, "Clear the screen"},
    {"ECHO", echo, "Print a message"},
    {"EXIT", shell_exit, "Halt the CPU"}
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