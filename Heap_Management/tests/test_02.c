/* test_02.c: allocate chunks and free middle */

#include <stdio.h>
#include <stdlib.h>

#define SIZE 1<<10

int main(int argc, char *argv[]) {
    int *p0 = malloc(SIZE);
    int *p1 = malloc(SIZE);
    int *p2 = malloc(SIZE);
    int *p3 = malloc(SIZE);

    free(p2);
    free(p1);
    p1 = malloc(SIZE);
    p2 = malloc(SIZE);

    free(p0);
    free(p1);
    free(p2);
    free(p3);

    return EXIT_SUCCESS;
}
