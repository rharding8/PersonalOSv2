#include "./../drivers/keyboard.h"
#include "./../drivers/screen.h"
#include "./../cpu/types.h"
#include "./../interpreter/interpreter.h"

static void read_line(char* buffer) {
    int i = 0;
    while (1) {
        char c = get_char();
        if (!c) continue; // Invalid or no character read, skip
        if (c == '\n') {
            buffer[i] = '\0';
            kline_break();
            return; // End of line, null-terminate the buffer and return
        }
        if (c == '\b') {
            if (i > 0) {
                i--;
                clear_one_character();
            }
            continue; // Handle backspace: move back in the buffer and clear the character on screen
        }
        buffer[i++] = c; // Store the character in the buffer
        kprint_char(c); // Echo the character to the screen
    }
}

int shell_run() {
    char input[128];
    int exit_flag = 0; // Flag to determine when to exit the shell
    while (!exit_flag) {
        kprint(">  ");
        clear_one_character(); // Clear the extra space after the prompt (Keeps cursor visible)
        read_line(input);
        exit_flag = interpret(input); // Interpret the input and get the exit flag
        kline_break();
    }
    if (exit_flag == 1) {
        return 1; // Signal to shutdown
    }
    return 0; // Signal to reset
}