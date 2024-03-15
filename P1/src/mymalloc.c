#include <stdint.h>
#include <stdio.h>
#include "mymalloc.h"


// smallest possible allocation size
#define MINALLOC 8

static long long mem[MEMSZ] = {0x1000001FF}; // Initialise top "pointer" + top chunk.

#define heapstart (chunk_header*) (mem)
#define heapend (chunk_header*) (mem + MEMSZ) 

#define NEXTCHUNK(chunk) (chunk_header*) (((char*) chunk) + 8 + (*(uint16_t *)(chunk)) * 8)
#define PREVCHUNK(chunk) (chunk_header*) (((char*) chunk) - 8 - (*(uint16_t *) (((char*) (chunk))+2)) * 8)

chunk_header* prevchunk(chunk_header* chunk) { 
    chunk_header* prv = PREVCHUNK(chunk);
    if (chunk->prevlength == 0 || prv < heapstart) return NULL;
    return prv;
}

chunk_header* nextchunk(chunk_header* chunk) {
    chunk_header* nxt = NEXTCHUNK(chunk);
    if (nxt < heapend) return nxt;
    else return NULL;
}

// Chunk:
// Bytes 0-1: Size. Byte 2: Free?
// 8 bytes of metadata (5 unused)

void* mymalloc(size_t sz, const char* file, int line) {
    #define ERROR(error) {fprintf(stderr, "mymalloc: %s (%s:%d)\n", error, file, line); return NULL;}
    sz = (sz + 7) & ~7; // Force sz to be a multiple of 8.
    if (sz < MINALLOC) sz = MINALLOC; // Force sz to be at least 8.
    size_t sz_lines = sz / 8;
    chunk_header* current_chunk = heapstart;
    while (current_chunk) {
        uint16_t chunk_lines = current_chunk->length;
        bool is_free = current_chunk->free;
        if (is_free) {
            if (chunk_lines == sz_lines) {
                current_chunk->free = 0;
                return &current_chunk->data;
            } else if (chunk_lines > sz_lines) {
                current_chunk->free = 0;
                chunk_header* new_chunk = (chunk_header*) ((char*) current_chunk + sz_lines * 8 + 8);
                if (chunk_lines <= sz_lines + 1) {
                    // A new chunk would have no lines in it.
                    // Do not bother creating a new chunk. Serve the allocation as-is.
                    return &current_chunk->data;
                }
                current_chunk->length = sz_lines;
                new_chunk->length = chunk_lines - sz_lines - 1;
                new_chunk->prevlength = sz_lines; 
                new_chunk->free = 1;
                chunk_header* next = nextchunk(new_chunk);
                if (next) next->prevlength = new_chunk->length;
                return &current_chunk->data;
            }
        }
        current_chunk = nextchunk(current_chunk);
    }
    return NULL;
    #undef ERROR
}

void myfree(void *ptr, const char *file, int line) {
    #define ERROR(error) {fprintf(stderr, "myfree: %s (%s:%d)\n", error, file, line); exit(1);} 
    #define INFO(error) {fprintf(stderr, "myfree: %s (%s:%d)\n", error, file, line);} 
    if ((void*) heapstart > ptr || (void*) heapend <= ptr) ERROR("not a heap pointer");
        chunk_header* current_chunk = heapstart;
    chunk_header* target_chunk = (chunk_header*) (((char*) ptr) - 8);
    while (current_chunk && current_chunk != target_chunk) {
        current_chunk = nextchunk(current_chunk);
    }
    if (current_chunk == NULL) ERROR("no such chunk")
    if (current_chunk->free) ERROR("double free")
    current_chunk->free = 1;
    chunk_header* prev = prevchunk(current_chunk);
    chunk_header* next = nextchunk(current_chunk);
    if (next && next->free) {
        // Coalesce forward.
        if (next->prevlength != current_chunk->length) {
            printf("%p %p, %d %d\n", current_chunk, next, current_chunk->length, next->prevlength);
            ERROR("length mismatch next, heap corrupt")
        }
        current_chunk->length = next->length + current_chunk->length + 1;
        next = nextchunk(current_chunk);
        if (next) next->prevlength = current_chunk->length;
    }
    if (prev && prev->free) {
        // Coalesce backwards.
        if (prev->length != current_chunk->prevlength) {
            ERROR("length mismatch prev, heap corrupt")
        }
        prev->length = prev->length + current_chunk->length + 1;
        if (next) {
            next->prevlength = prev->length;
        }
    }
    // We're done here!
    #undef ERROR
}