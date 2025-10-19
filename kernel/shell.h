#ifndef SHELL_H
#define SHELL_H

void command_parser(char *input);
void help();
void unknown_command();
void clear();
void echo(char *args);
void shell_exit();

#endif

