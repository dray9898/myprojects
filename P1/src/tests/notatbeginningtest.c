#include "../testheaders.h"

#define TESTNAME "notatbeginning"

void run_test() {
    char* c= malloc(100);
    c= c+8;
    free(c);
}

int main() {
    char filename[80];
    strcpy(filename, TESTNAME);
    freopen(strcat(filename, "_result.txt"), "w", stderr);
    run_test();
}