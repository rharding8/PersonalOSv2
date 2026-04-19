#include "interpreter.h"
#include "../libc/util.h"
#include "../drivers/screen.h"
#include "../memory/memory.h"

typedef struct {
    char name;
    int* value_ptr;
} Variable;

Variable vars[26]; // A-Z

int interpret(char* input) {
    if (strncmp(input, "PEEK ", 5) == 0) {
        char *addr_str = input + 5;
        int addr;
        ascii_to_int(addr_str, &addr);
        int value = peek_block((void*)addr);
        char buf[16];
        int_to_ascii(value, buf);
        kprint(buf);
        kprint("\n");
        return 0;
    }

    if (strncmp(input, "POKE ", 5) == 0) {
        char *addr_str = input + 5;
        char *val_str = addr_str;
        while (*val_str != ' ' && *val_str != '\0') val_str++;
        if (*val_str == ' ') {
            *val_str = '\0'; // Null-terminate the address string
            val_str++; // Move to the value string
        } else {
            kprint("INVALID POKE SYNTAX\n");
            return 0; 
        }
        int addr, value;
        ascii_to_int(addr_str, &addr);
        ascii_to_int(val_str, &value);
        poke_block((void*)addr, value);
        kprint("POKED\n");
        return 0;
    }

    if (strncmp(input, "LET ", 4) == 0) {
        char var = input[4];       // 'A' to 'Z'
        int value = input[8] - '0'; // Assuming single digit for simplicity

        int* mem = (int*)alloc_block();
        *mem = value;
        vars[var - 'A'].value_ptr = mem;

        kprint("STORED\n");
        return 0;
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
            return 0;
        } else if (c == '\"') {
            c = input[7]; // Get the character inside quotes
            input += 7; // Move past "PRINT "
            while (c != '\"') {
                kprint_char(c);
                input++;
                c = *input;
            }
            kprint("\n");
            return 0;
        }

        int value = *vars[c - 'A'].value_ptr;

        char buf[16];
        int_to_ascii(value, buf);

        kprint(buf);
        kprint("\n");
        return 0;
    }

    if (strcmp(input, "CLEAR") == 0) {
        clear_screen();
        return;
    }

    if (strcmp(input, "EXIT") == 0) {
        kprint("SHUTTING DOWN...\n");
        return 1; // Signal to shutdown
    }

    if (strcmp(input, "RESET") == 0) {
        kprint("REBOOTING...\n");
        return 2; // Signal to reboot
    }

    kprint("UNKNOWN COMMAND\n");
    return 0; // Signal to continue
}