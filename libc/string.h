#ifndef STRINGS_H
#define STRINGS_H

#include "../cpu/types.h"

void int_to_ascii(s32 n, char str[]);
void hex_to_ascii(s32 n, char str[]);
void reverse(char s[]);
s32 strlen(char s[]);
void backspace(char s[]);
void append(char s[], char n);
s32 strcmp(char s1[], char s2[]);

#endif
