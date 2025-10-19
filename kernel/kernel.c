#include "../cpu/isr.h"
#include "../drivers/screen.h"
#include "kernel.h"
#include "shell.h"

void main() {
    isr_install();
    irq_install();

    clear_screen();
    kprint("> ");
}

void user_input(char *input) {
    command_parser(input);
    kprint("> ");
}