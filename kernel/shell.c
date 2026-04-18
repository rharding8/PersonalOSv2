#include "./../drivers/keyboard.h"
#include "./../drivers/screen.h"
#include "./../cpu/types.h"

static void read_line(char* buffer) {
    int i = 0;

    while (1) {
        char c = get_char();

        if (!c) continue;

        if (c == '\n') {
            buffer[i] = '\0';
            kline_break();
            return;
        }

        if (c == '\b') {
            if (i > 0) {
                i--;
                clear_one_character();
            }
            continue;
        }

        buffer[i++] = c;
        kprint_char(c);
    }
}

void shell_run() {
    char input[128];
    while (1) {
        kprint(">  ");
        clear_one_character();
        read_line(input);
    }
}