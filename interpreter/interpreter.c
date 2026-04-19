#include "interpreter.h"
#include "../libc/util.h"
#include "../drivers/screen.h"
#include "../memory/memory.h"

typedef struct {
    char name;
    int* value_ptr;
} Variable;

Variable vars[26]; // A-Z

void interpret(char* input) {
    if (strncmp(input, "PEEK ", 5) == 0) {
        char *addr_str = input + 5;
        int addr;
        ascii_to_int(addr_str, &addr);
        int value = peek_block((void*)addr);
        char buf[16];
        int_to_ascii(value, buf);
        kprint(buf);
        kprint("\n");
        return;
    }

    if (strncmp(input, "LET ", 4) == 0) {
        char var = input[4];       // 'A' to 'Z'
        int value = input[8] - '0'; // Assuming single digit for simplicity

        int* mem = (int*)alloc_block();
        *mem = value;
        vars[var - 'A'].value_ptr = mem;

        kprint("STORED\n");
        return;
    }

    if (strncmp(input, "PRINT ", 6) == 0) {
        char c = input[6];
        if (c == '\'') {
            c = input[7]; // Get the character inside quotes
            input += 7; // Move past "PRINT "
            while (c != '\'') {
                kprint_char(c);
                input++;
                c = *input;
            }
            kprint("\n");
            return;
        } else if (c == '\"') {
            c = input[7]; // Get the character inside quotes
            input += 7; // Move past "PRINT "
            while (c != '\"') {
                kprint_char(c);
                input++;
                c = *input;
            }
            kprint("\n");
            return;
        }

        int value = *vars[c - 'A'].value_ptr;

        char buf[16];
        int_to_ascii(value, buf);

        kprint(buf);
        kprint("\n");
        return;
    }

    if (strcmp(input, "CLEAR") == 0) {
        clear_screen();
        return;
    }

    kprint("UNKNOWN COMMAND\n");
}