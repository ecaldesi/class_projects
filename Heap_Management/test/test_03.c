/* test_03.c: allocate chunks of different sizes */

#include <stdio.h>
#include <stdlib.h>

#define N    10

int main(int argc, char *argv[]) {
    char *p[N];

    for (int i = 0; i < N; i++)
    	p[i] = malloc(1<<(N - i));
    
    for (int i = 0; i < N; i+=2)
    	free(p[i]);
    
    for (int i = 0; i < N; i++)
    	p[i] = malloc(1<<i);
    
    for (int i = 1; i < N; i+=2)
    	free(p[i]);
    
    for (int i = 0; i < N; i++)
    	p[i] = malloc(1<<(N - i));

    return EXIT_SUCCESS;
}
