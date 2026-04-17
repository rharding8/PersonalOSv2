#include "loader.h"
#include "memory.h"
#include "./../cpu/types.h"
#include "./../libc/util.h"

void* load(uchar_8* program, int size) {
    void* addr = alloc_block();

    if (!addr) return 0;

    memcpy_pos(program, addr, size);

    return addr;
}