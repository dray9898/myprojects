#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
// Compile with -DREALMALLOC to use the real malloc() instead of mymalloc()
#ifndef REALMALLOC
#include "mymalloc.h"
#endif
#define MEMSIZE 4096
#define HEADERSIZE 8
#define OBJECTS 64
#define OBJSIZE (MEMSIZE / OBJECTS - HEADERSIZE)