/* test_00.c: allocate same size block over and over again */

#include <stdlib.h>

#define N	10
#define SIZE	1<<10

int main(int argc, char *argv[]) {
    for (int i = 0; i < N; i++) {
    	int *p = malloc(SIZE);
    	free(p);
    }

    return EXIT_SUCCESS;
}
