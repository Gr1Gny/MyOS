#include "shell.h"
#include "../drivers/screen.h"
#include "../libc/string.h"

void command_parser(char *input) {
    char *args = NULL;

    int i = 0;
    while (input[i] != '\0') {
        if (input[i] == ' ') {
            input[i] = '\0'; 
            args = &input[i+1];
            break;
        }
        i++;
    }

    if (strcmp(input, "HELP") == 0) {
            help();
    }
    else if (strcmp(input, "CLEAR") == 0) {
        clear();
    }
    else if (strcmp(input, "ECHO") == 0) {
        echo(args);
    }
    else if (strcmp(input, "EXIT") == 0) {
        shell_exit();
    }
    else if (strcmp(input, "") != 0) {
        unknown_command();
    }
}

void help() {
    kprint("Available commands:\n");
    kprint("help - Show this help message\n");
    kprint("clear - Clear the screen\n");
    kprint("echo - Print a message\n");
    kprint("exit - Exit the shell\n");
}

void unknown_command() {
    kprint("Unknown command\n");
}

void clear() {
    clear_screen();
}

void echo(char *args) {
    kprint(args);
    kprint("\n");
}

void shell_exit() {
    __asm__ __volatile__("hlt");
}