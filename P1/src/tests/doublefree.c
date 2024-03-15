#include <stdio.h>
#include <string.h>

int main() {
    FILE *file;
    char buffer[100];

    file = fopen("doublefree_result.txt", "r");

    
    int found =0;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (strstr(buffer, "double free") != NULL) {
            printf("Found 'double free' in the line: %s", buffer);
            found=1;
        }
    }
    if(!found){
        printf("Did not find 'double free' in the line: %s", buffer);
    }

    // Close the file
    fclose(file);

    return 0;
}
