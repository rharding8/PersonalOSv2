#ifndef PROCESS_H
#define PROCESS_H

typedef struct {
    void* entry;
    void* stack;
} Process;

void run(Process* p);

#endif