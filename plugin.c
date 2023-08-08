#include <stdio.h>
#include <string.h>

int initialize() {
    return 0;
}

int run(char ** argv) {
    printf("Executing command: ");
    for (int i = 0; argv[i] != NULL; i++) {
        printf("%s ", argv[i]);
    }
    printf("\n");
    return 0; // return 0 to indicate success

}
