#include <stdio.h>
#include <string.h>

int main() {
    FILE *file;
    char buffer[100];

    file = fopen("notaheappointer_result.txt", "r");

    
    int found =0;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (strstr(buffer, "not a heap pointer") != NULL) {
            printf("Not a heap pointer freed in the line: %s", buffer);
            found=1;
        }
    }
    if(!found){
        printf("This line didd not free a non-heap pointer: %s", buffer);
    }

    // Close the file
    fclose(file);

    return 0;
}

