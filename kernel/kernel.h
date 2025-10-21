#ifndef KERNEL_H
#define KERNEL_H

#define HISTORY_SIZE 10
#define PROMPT_TEXT "MyOs> "

void user_input(char *input);
void add_to_history(char *cmd);
char* get_history(int offset);
char* tab_complete(char *partial);
char get_input_color();
void set_input_color(char color);

#endif
