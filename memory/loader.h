#ifndef LOADER_H
#define LOADER_H

#include "./../cpu/types.h"
#include "./../libc/util.h"

void* load(uchar_8* program, int size);

#endif