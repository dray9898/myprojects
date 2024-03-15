#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// Compile with -DREALMALLOC to use the real malloc() instead of mymalloc()
#ifndef REALMALLOC
#include "mymalloc.h"
#endif
#define MEMSIZE 4096
#define HEADERSIZE 8
#define OBJECTS 64
#define OBJSIZE (MEMSIZE / OBJECTS - HEADERSIZE)

struct MyStruct {
    char c1;
    int arr[5];
    char c2;
    double d;
};

void init() {
    srand(time(NULL));
}

void test1() {
    for (int i = 0; i < 120; i++) {
        free(malloc(1));
    }
}

void test2() {
    char* arr[120];
    for (int i = 0; i < 120; i++) {
        arr[i] = malloc(1);
    }
    for (int i = 0; i < 120; i++) {
        free(arr[i]);
    }
}

void test3() {
    char* arr[120];
    int alloc_count = 0, total_alloc = 0;
    while (total_alloc < 120) {
        if (alloc_count == 0 || rand() & 1) {
            //printf("MALLOC %d\n", alloc_count);
            arr[alloc_count++] = malloc(1);
            total_alloc++;
        } else {
            int index = rand() % alloc_count;
            //printf("FREE %d\n", index);
            free(arr[index]);
            arr[index] = arr[--alloc_count];
        }
    }
    while (alloc_count) {
        free(arr[--alloc_count]);
    }
}

void test4() {
    char* arr[10];
    int alloc_count = 0, total_alloc = 0;
    while (total_alloc < 120) {
        if (alloc_count == 0 || (alloc_count != 10 && (rand() & 1))) {
            //printf("MALLOC %d\n", alloc_count);
            arr[alloc_count++] = malloc(400); // Bigger allocations
            total_alloc++;
        } else {
            int index = rand() % alloc_count;
            //printf("FREE %d\n", index);
            free(arr[index]);
            arr[index] = arr[--alloc_count];
        }
    }
    while (alloc_count) {
        free(arr[--alloc_count]);
    }
}
void test5() {
    for(int i=0;i<10000;i++){
        int n = 3; // Number of structs to allocate
        //Dynamically allocate memory for an array of n structs
        struct MyStruct *ptr = (struct MyStruct*)malloc(n * sizeof(struct MyStruct));
        // Initializes n structs based on a simple algorithm

        for (int i = 0; i < n; i++) {
            ptr[i].c1 = 'A' + i;
            for (int j = 0; j < 5; j++) {
            ptr[i].arr[j] = j + 1;
        }
            ptr[i].c2 = 'F' + i;
            ptr[i].d = 3.14 * (i + 1);        
    }
    free(ptr);
}
}

void run_test(const char* name, void func(void), int iterations) {
    double total_time = 0;
    for (int i = 0; i < iterations; i++) {
        clock_t start = clock();
        func();
        clock_t end = clock();
        total_time += (double) (end - start) / CLOCKS_PER_SEC;
    }
    printf("%s: %lf seconds on average, %lf seconds total\n", name, total_time / iterations, total_time);
}

int main() {
    init();
    run_test("allocfree x 120", test1, 10000);
    run_test("alloc x120 then free x120", test2, 10000);
    run_test("randomised test", test3, 10000);
    run_test("randomised (bigger allocations)", test4, 10000);
     run_test("struct allocation x10000", test5, 10000);

}