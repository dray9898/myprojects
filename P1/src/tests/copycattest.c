#include "../testheaders.h"

#define TESTNAME "copycat"

void run_test() {
    char* a = malloc(4000);
    memmove(a, a - 8, 4000);
    free(a+8);
}

int main() {
    char filename[80];
    strcpy(filename, TESTNAME);
    freopen(strcat(filename, "_result.txt"), "w", stderr);
    run_test();
}