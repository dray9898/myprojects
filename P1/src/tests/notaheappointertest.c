#include "../testheaders.h"

#define TESTNAME "notaheappointer"

void run_test() {
    double a = 1;
    double *s = &a;
    free(s);
}

int main() {
    char filename[80];
    strcpy(filename, TESTNAME);
    freopen(strcat(filename, "_result.txt"), "w", stderr);
    run_test();
}