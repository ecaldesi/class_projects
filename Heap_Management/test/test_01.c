/* test_01.c: allocate descending sized blocks */

#include <stdio.h>
#include <stdlib.h>

#define SIZE	1<<10

int main(int argc, char *argv[]) {
    for (size_t s = SIZE; s > 0; s>>=1) {
    	int *p = malloc(s);
    	free(p);
    }

    return EXIT_SUCCESS;
}
