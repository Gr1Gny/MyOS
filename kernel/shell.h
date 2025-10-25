#ifndef SHELL_H
#define SHELL_H

#define NUM_COMMANDS 6

typedef void (*command_handler_t)(char *args);

typedef struct {
    char *name;                    // Command name
    command_handler_t handler;     // Function to call
    char *description;
} command_t;

void command_parser(char *input);
void unknown_command();

/* all take char* args for consistency */
void help(char *args);
void clear(char *args);
void echo(char *args);
void shell_exit(char *args);
void prompt(char *args);
void mem(char *args);

#endif

