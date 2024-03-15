#include <stdio.h>
#include <string.h>

int main() {
    FILE *file;
    char buffer[100];

    file = fopen("copycat_result.txt", "r");

    
    int found =0;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (strstr(buffer, "no such chunk") != NULL) {
            printf("Found 'no such chunk' allocated in the line: %s", buffer);
            found=1;
        }
    }
    if(!found){
        printf("Failed to error\n");
    }

    // Close the file
    fclose(file);

    return 0;
}

