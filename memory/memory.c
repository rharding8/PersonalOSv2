#include "memory.h"
#include "./../cpu/types.h"
#include "./../libc/util.h"

#define BLOCKSIZE 4096
#define NBLOCKS 1024

static uchar_8 bitmap[NBLOCKS];

void init_memory() {
    int i;
    for (i = 0; i < NBLOCKS; i++) {
        bitmap[i] = 0;
    }

    // Reserve first few blocks (kernel, low memory, etc.)
    for (i = 0; i < 10; i++) {
        bitmap[i] = 1;
    }
}

void* alloc_block() {
    int i;
    for (i = 0; i < NBLOCKS; i++) {
        if (bitmap[i] == 0) {
            bitmap[i] = 1;
            return (void*)(i * BLOCKSIZE);
        }
    }
    return 0;
}

int peek_block(void* addr) {
    int index = (uint_32)addr / BLOCKSIZE;
    if (index < NBLOCKS && bitmap[index] == 1) {
        return *((int*)addr);
    }
    return -1; // Invalid address
}

void free_block(void* addr) {
    int index = (uint_32)addr / BLOCKSIZE;
    bitmap[index] = 0;
}