#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

void init_memory();
void* alloc_block();
void free_block(void* addr);

#endif