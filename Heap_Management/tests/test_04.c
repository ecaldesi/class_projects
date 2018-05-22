/* test_04.c: allocate lots of memory */

#include <stdio.h>
#include <stdlib.h>

#define N    (1<<15)

int main(int argc, char *argv[]) {
    char **p = malloc(sizeof(char *) * N);

    for (int i = 0; i < N; i++)
    	p[i] = malloc(1);
    
    for (int i = 0; i < N; i+=2)
    	free(p[i]);
    
    for (int i = 0; i < N; i++)
    	p[i] = malloc(1);
    
    for (int i = 1; i < N; i+=2)
    	free(p[i]);
    
    for (int i = 0; i < N; i++)
    	p[i] = malloc(1);

    return EXIT_SUCCESS;
}
