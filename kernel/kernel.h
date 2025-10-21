#ifndef KERNEL_H
#define KERNEL_H

#define HISTORY_SIZE 10

void user_input(char *input);
void add_to_history(char *cmd);
char* get_history(int offset);
void replace_input_line(char *new_input);
char* tab_complete(char *partial);
char get_prompt_color();
void set_prompt_color(char color);

#endif
