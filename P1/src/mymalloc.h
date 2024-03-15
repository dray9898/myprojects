#pragma once
#ifndef _MYMALLOC_H
#define _MYMALLOC_H
#include <stdlib.h>
#include <stdbool.h>


#define MEMSZ 512
// MEMSIZE must be less than 2^15.

typedef struct {
    short length, prevlength;
    bool free;
    long long data;
} chunk_header;

#define malloc(s) mymalloc(s, __FILE__, __LINE__)
#define free(p) myfree(p, __FILE__, __LINE__)
void *mymalloc(size_t size, const char *file, int line);
void myfree(void *ptr, const char *file, int line);
#endif