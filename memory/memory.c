#include "memory.h"
#include "./../cpu/types.h"
#include "./../libc/util.h"

#define BLOCKSIZE 4096
#define NBLOCKS 1024

static uchar_8 bitmap[NBLOCKS];

typedef struct heap_block {
    int size;
    int free;
    struct heap_block* next;
} heap_block_t;

static heap_block_t* heap_start = 0;

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

void heap_init() {
    heap_start = (heap_block_t*)alloc_block();

    heap_start->size = 4096 - sizeof(heap_block_t);
    heap_start->free = 1;
    heap_start->next = 0;
}

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

int peek_block(void* addr) {
    int index = (uint_32)addr / BLOCKSIZE;
    if (index < NBLOCKS && bitmap[index] == 1) {
        return *((int*)addr);
    }
    return -1; // Invalid address
}

void poke_block(void* addr, int value) {
    int index = (uint_32)addr / BLOCKSIZE;
    if (index < NBLOCKS && bitmap[index] == 1) {
        *((int*)addr) = value;
    }
}

void free_block(void* addr) {
    int index = (uint_32)addr / BLOCKSIZE;
    bitmap[index] = 0;
}

void* kmalloc(int size) {
    heap_block_t* curr = heap_start;

    while (curr) {
        if (curr->free && curr->size >= size) {
            curr->free = 0;
            return (void*)(curr + 1);
        }

        if (!curr->next) break;
        curr = curr->next;
    }

    // Need new block
    heap_block_t* new_block = (heap_block_t*)alloc_block();

    new_block->size = 4096 - sizeof(heap_block_t);
    new_block->free = 0;
    new_block->next = 0;

    curr->next = new_block;

    return (void*)(new_block + 1);
}

void kfree(void* ptr) {
    if (!ptr) return;

    heap_block_t* block = (heap_block_t*)ptr - 1;
    block->free = 1;
}