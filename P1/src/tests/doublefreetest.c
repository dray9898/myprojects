#include "../testheaders.h"

#define TESTNAME "doublefree"

void run_test() {
    char* a = malloc(4000);
    free(a);
    free(a);
}

int main() {
    char filename[80];
    strcpy(filename, TESTNAME);
    freopen(strcat(filename, "_result.txt"), "w", stderr);
    run_test();
}