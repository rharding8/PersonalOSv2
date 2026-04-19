
#ifndef __KERNEL_UTIL_H__
#define __KERNEL_UTIL_H__
#include "./../cpu/types.h"
void memcpy_pos(char * source, char * dest, int bytes);
void memset_pos(uchar_8* dest, uchar_8 value, uint_32 length);
void int_to_ascii(int n, char str[]);

void reverse(char s[]);
int strlen(char s[]);

int strcmp(char* a, char* b);
int strncmp(char* a, char* b, int n);

#endif