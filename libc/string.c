#include "string.h"

void int_to_ascii(s32 n, char str[]) {
    s32 i, sign;
    if ((sign = n) < 0) n = -n;
    i = 0;
    do {
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0) str[i++] = '-';
    str[i] = '\0';

    reverse(str);
}

void hex_to_ascii(s32 n, char str[]) {
    append(str, '0');
    append(str, 'x');
    u8 zeros = false;

    s32 tmp;
    s32 i;
    for (i = 28; i > 0; i -= 4) {
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && !zeros) continue;
        zeros = true;
        if (tmp > 0xA) append(str, tmp - 0xA + 'a');
        else append(str, tmp + '0');
    }

    tmp = n & 0xF;
    if (tmp >= 0xA) append(str, tmp - 0xA + 'a');
    else append(str, tmp + '0');
}

void reverse(char s[]) {
    s32 c, i, j;
    for (i = 0, j = strlen(s)-1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

s32 strlen(char s[]) {
    s32 i = 0;
    while (s[i] != '\0') ++i;
    return i;
}

void append(char s[], char n) {
    s32 len = strlen(s);
    s[len] = n;
    s[len+1] = '\0';
}

void backspace(char s[]) {
    s32 len = strlen(s);
    s[len-1] = '\0';
}

/* Returns <0 if s1<s2, 0 if s1==s2, >0 if s1>s2 */
s32 strcmp(char s1[], char s2[]) {
    s32 i;
    for (i = 0; s1[i] == s2[i]; i++) {
        if (s1[i] == '\0') return 0;
    }
    return s1[i] - s2[i];
}

void strcpy(char dest[], char src[]) {
    s32 i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

s32 strncmp(char s1[], char s2[], s32 n) {
    s32 i;
    for (i = 0; i < n && s1[i] == s2[i]; i++) {
        if (s1[i] == '\0') return 0;
    }
    if (i == n) return 0;
    return s1[i] - s2[i];
}
