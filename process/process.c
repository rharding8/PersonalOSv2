#include "process.h"

void run(Process* p) {
    // Set stack pointer
    asm volatile ("mov %0, %%esp" :: "r"(p->stack));

    // Jump to program
    void (*entry)() = p->entry;
    entry();

    while (1); // prevent return crash
}