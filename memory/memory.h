#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

void init_memory();
void* alloc_block();
int peek_block(void* addr);
void poke_block(void* addr, int value);
void free_block(void* addr);

#endif