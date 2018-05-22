/* unit_test_00.c: test find_free */

#include <stdlib.h>
#include "../src/malloc.c"

#define N	10
#define SIZE	1<<10

int main(int argc, char *argv[]) {
    for (int i = 0; i < N; i++) {
    	int *p = malloc(SIZE);
    	free(p);
    }
    
    return EXIT_SUCCESS;
}

/*
int main(int argc, char *argv[]) {
	struct block * head = malloc(sizeof(struct block));
	struct block * second = malloc(sizeof(struct block));
	struct block * third = malloc(sizeof(struct block));
	struct block * fourth = malloc(sizeof(struct block));
	struct block * fifth = malloc(sizeof(struct block));
	struct block * sixth = malloc(sizeof(struct block));

	head->size = ALIGN4(304);
	head->free = true;
	head->next = second;

	second->size = ALIGN4(148);
	second->free = false;
	second->next = third;

	third->size = ALIGN4(204);
	third->free = true;
	third->next = second;

	fourth->size = ALIGN4(44);
	fourth->free = false;
	fourth->next = fifth;

	fifth->size = ALIGN4(96);
	fifth->free = false;
	fifth->next = sixth;

	sixth->size = ALIGN4(264);
	sixth->free = true;
	sixth->next = NULL;

   	struct block **last;
	*last = third;
	
	struct block *ptr = find_free(last, 85);	

	struct block *curr = head;
	int counter = 0;
	if (ptr) {
		while(curr) {
			counter++;
			if (ptr->size == curr->size) {
				return counter;
			}
		}
		counter = 0;
	}
	free(head);
	free(second);
	free(third);
	free(fourth);
	free(fifth);
	free(sixth);

	char buffer[BUFSIZ];
	sprintf(buffer, "%d\n", counter);
	write(STDOUT_FILENO, buffer, strlen(buffer));
	return counter;	 
}
*/
