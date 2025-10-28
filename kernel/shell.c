#include "shell.h"
#include "../drivers/screen.h"
#include "../libc/string.h"
#include "../libc/mem.h"
#include "kernel.h"
#include "../libc/function.h"
#include "../cpu/paging.h"

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

void mem(char *args) {
    UNUSED(args);
    
    char num_str[16];
    
    /* Physical memory (frames) */
    u32 used_frames = get_used_frame_count();
    u32 free_frames = get_free_frame_count();
    u32 total_frames = used_frames + free_frames;
    
    kprintf_color(get_input_color(), "Physical Memory (Frames):\n");
    kprintf_color(get_input_color(), "  Total: %d frames (%d KB)\n", total_frames, total_frames * 4);
    
    kprintf_color(get_input_color(), "  Used:  %d frames (%d KB)\n", used_frames, used_frames * 4);
    
    kprintf_color(get_input_color(), "  Free:  %d frames (%d KB)\n", free_frames, free_frames * 4);
    
    /* Kernel heap */
    u32 heap_total, heap_used, heap_free;
    get_heap_stats(&heap_total, &heap_used, &heap_free);
    
    kprintf_color(get_input_color(), "\nKernel Heap:\n");
    kprintf_color(get_input_color(), "  Total: %d bytes (%d KB)\n", heap_total, heap_total / 1024);
    
    kprintf_color(get_input_color(), "  Used:  %d bytes (%d KB)\n", heap_used, heap_used / 1024);
    
    kprintf_color(get_input_color(), "  Free:  %d bytes (%d KB)\n", heap_free, heap_free / 1024);
}

void unknown_command() {
    kprint_color("Unknown command. Type 'help' for available commands.\n", get_input_color());
}

/* Command table - defined AFTER the functions so they exist */
command_t commands[] = {
    {"help", help, "Show this help message"},
    {"clear", clear, "Clear the screen"},
    {"echo", echo, "Print a message"},
    {"mem", mem, "Show memory statistics"},
    {"prompt", prompt, "Change typing color"},
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