#include "interpreter.h"
#include "../libc/util.h"
#include "../drivers/screen.h"
#include "../memory/memory.h"

typedef enum {
    VAR_INT,
    VAR_ARRAY
} VarType;

typedef struct {
    char name[16];
    VarType type;
    void* data;
    int size;
} Variable;

#define MAX_VARS 64

static Variable variables[MAX_VARS];
static int var_count = 0;

Variable* find_var(const char* name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            return &variables[i];
        }
    }
    return 0;
}

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

    if (strncmp(input, "let ", 4) == 0) {
        char var_name[16];
        int value;

        // VERY naive parsing
        var_name[0] = input[4];
        var_name[1] = '\0';

        ascii_to_int(input + 8, &value);

        Variable* v = find_var(var_name);

        if (!v) {
            v = &variables[var_count++];
        }

        strcpy(v->name, var_name);
        v->type = VAR_INT;
        v->data = kmalloc(sizeof(int));

        *(int*)v->data = value;

        kprint("OK\n");
        return;
    }

    if (strncmp(input, "array ", 6) == 0) {
        char name[16];
        int size;

        name[0] = input[6];
        name[1] = '\0';

        ascii_to_int(input + 8, &size);

        Variable* v = &variables[var_count++];

        strcpy(v->name, name);
        v->type = VAR_ARRAY;
        v->size = size;

        v->data = kmalloc(sizeof(int) * size);

        // Initialize to 0
        for (int i = 0; i < size; i++) {
            ((int*)v->data)[i] = 0;
        }

        kprint("Array created\n");
        return;
    }

    if (strncmp(input, "set ", 4) == 0) {
        char name[16];
        int index, value;

        name[0] = input[4];
        name[1] = '\0';

        ascii_to_int(input + 6, &index);
        ascii_to_int(input + 8, &value);

        Variable* v = find_var(name);

        if (!v || v->type != VAR_ARRAY) {
            kprint("Invalid array\n");
            return;
        }

        if (index >= v->size) {
            kprint("Out of bounds\n");
            return;
        }

        ((int*)v->data)[index] = value;

        kprint("OK\n");
        return;
    }

    if (strncmp(input, "get ", 4) == 0) {
        char name[16];
        int index;

        name[0] = input[4];
        name[1] = '\0';

        ascii_to_int(input + 6, &index);

        Variable* v = find_var(name);

        if (!v || v->type != VAR_ARRAY) {
            kprint("Invalid array\n");
            return;
        }

        int value = ((int*)v->data)[index];

        char buf[16];
        int_to_ascii(value, buf);

        kprint(buf);
        kprint("\n");

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

        char var_name[16];

        var_name[0] = input[6];
        var_name[1] = '\0';

        Variable* v = find_var(var_name);

        if (!v) {
            kprint("Undefined\n");
            return;
        }

        if (v->type == VAR_INT) {
            char buf[16];
            int_to_ascii(*(int*)v->data, buf);
            kprint(buf);
            kprint("\n");
        }

        return;
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