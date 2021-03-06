/* malloc.c: simple memory allocator -----------------------------------------*/

#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/* Macros --------------------------------------------------------------------*/

#define ALIGN4(s)           (((((s) - 1) >> 2) << 2) + 4)
#define BLOCK_DATA(b)       ((b) + 1)
#define BLOCK_HEADER(ptr)   ((struct block *)(ptr) - 1)

/* Block structure -----------------------------------------------------------*/

struct block {
    size_t        size;
    struct block *next;
    bool          free;
};

/* Global variables ----------------------------------------------------------*/

struct block *FreeList = NULL;
bool end = true;
int newfd;
struct block * left_off = NULL;

/* TODO: Add counters for mallocs, frees, reuses, grows */
int malloc_counter = 0;
int free_counter = 0;
int recycle = 0;
int request_new = 0;
int split_counter = 0;
int coalesce_counter = 0;
int nblocks = 0;
size_t total_memory = 0;
size_t max_heap = 0;


/* Find number of blocks in Free list ------------------------------------------*/
void counters_update() {
    struct block *curr = FreeList;
    while (curr) {
        /* grow counter for the number of blocks */
        nblocks++;
        curr = curr->next;
    }
}


/* Print functions -------------------------------------------------------------*/
void print_results() {
    counters_update();
    char buffer[1<<10];
    sprintf(buffer, "mallocs:   %d\nfrees:     %d\nreuses:    %d\ngrows:     %d\nsplits:    %d\ncoalesces: %d\nblocks:    %d\nrequested: %lu\nmax heap:  %lu\n", malloc_counter, free_counter, recycle, request_new, split_counter, coalesce_counter, nblocks, total_memory, max_heap);
    write(newfd, buffer, strlen(buffer));
}

/*
void debug_print() {
    struct block* printBlock = FreeList;
    char buffy[1<<10];
    while(printBlock){
        if(printBlock->free){
            sprintf(buffy, "| %zu- O |\n", printBlock->size);
        }else{
            sprintf(buffy, "| %zu- X |\n", printBlock->size);
        }
        printBlock = printBlock->next;
        write(newfd, buffy, strlen(buffy));
    }
}
*/

/* Call atexit function ------------------------------------------------------*/
void call_atexit() {
    if (end == true) {
        atexit(print_results);
    } 
    end = false;
}

/* Find free block -----------------------------------------------------------*/

struct block *find_free(struct block **last, size_t size) {
    struct block *curr = FreeList;

#if defined FIT && FIT == 0
    /* First fit */
    while (curr && !(curr->free && curr->size >= size)) {
        *last = curr;
        curr  = curr->next;
    }
#endif


#if defined FIT && FIT == 1
    /* Best fit */
    struct block * temp = NULL;                         //temp = best fit so far
    while (curr) {                                      //check every block 
        if (curr->free && size <= curr->size) {
            if (!temp)                                 
                temp = curr;
            else if (curr->size < temp->size) 
                temp = curr;
        }
        *last = curr;
        curr = curr->next;
    }
                                                   
    curr = temp;    //If no block found, curr = NULL & Last = last block in list
#endif


#if defined FIT && FIT == 2
    /* Worst fit */
    struct block * temp = NULL;                         //temp = worst fit so far 
    while (curr) {                                      //check every block
        if (curr->free && size <= curr->size) {
            if (!temp) 
                temp = curr;
            else if (curr->size > temp->size) 
                temp = curr;
        }
        *last = curr;
        curr = curr->next;
    }

    curr = temp;    //If no block found, curr = NULL & Last = last block in list
#endif


#if defined FIT && FIT == 3			
    /* Next fit */
    
    if(!left_off) left_off = FreeList;
    curr = left_off; // initialize curr to the last block checked
    while (curr && !(curr->free && curr->size >= size)) {
        *last = curr;
        curr  = curr->next;
    }
    
    /* if you didn't find a block that was free */
    if (curr == NULL) {
        curr = FreeList;
        /* check list from the beginning */
        while (curr && !(curr->free && curr->size >= size) && (curr != left_off)) {
            curr  = curr->next;
        }
        if (curr == left_off) {
            curr = NULL;
        }
    }

    left_off = curr;        
#endif

    return curr;
}

/* Grow heap -----------------------------------------------------------------*/

struct block *grow_heap(struct block *last, size_t size) {
    /* grow counter for request memory */
    request_new++;

    /* Request more space from OS */
    struct block *curr = (struct block *)sbrk(0);
    struct block *prev = (struct block *)sbrk(sizeof(struct block) + size);

    max_heap = max_heap + size;
    assert(curr == prev);

    /* OS allocation failed */
    if (curr == (struct block *)-1) {
        return NULL;
    }
    
    /* Update FreeList if not set */
    if (FreeList == NULL) {
        FreeList = curr;
    }

    /* Attach new block to prev block */
    if (last) {
        last->next = curr;
    }

    /* Update block metadata */
    curr->size = size;
    curr->next = NULL;
    curr->free = false;
    return curr;
}

/* Allocate space ------------------------------------------------------------*/

void *malloc(size_t size) {

    //printf("Allocating Block of Size: %zu\n", size);

    /* insert atexit */
    call_atexit();
    newfd = dup(1);

    /* Align to multiple of 4 */
    size = ALIGN4(size);

    /* increase counter for memory requested */
    total_memory = total_memory + size;
    
    /* Handle 0 size */
    if (size == 0) {
        return NULL;
    }

    /* Look for free block */
    struct block *last = FreeList;
    struct block *next = find_free(&last, size);

    /* Find number of blocks before splitting */
    int temp_nblocks_before = 0;
    struct block *temp = FreeList;
    while (temp) {
        temp_nblocks_before++;
        temp = temp->next;
    }


    /* TODO: Split free block? */
    if ((next != NULL)){
        recycle++;                                                      //Iterate block reuse counter       
        
        /* If below condition is true, we have enough space to split the block and create a new block
        1. The free size of the new block is the leftover free space, minus the space needed to store the new block itself
        2. The address of a blocks neighbor is that blocks address, plus the size of a block, plus the size of that blocks space
        3. Once we calculate the size of the new block and it's address, we can set it's values*/

 
        if(sizeof(struct block) < (next->size - size)){                 
            split_counter++;                                                                //Iterate block split counter
            
            size_t new_size = next->size - size - sizeof(struct block);                              // 1 
            struct block *new_block = (void*)((size_t)next + sizeof(struct block) + (size_t)size);   // 2
            new_block->size = new_size;                                                              // 3
            new_block->next = next->next;
            new_block->free = true;
              
            next->size = size;
            next->next = new_block;

            /* Find number of blocks after splitting */
            int temp_nblocks_after = 0;
            temp = FreeList;
            while (temp) {
                temp_nblocks_after++;
                temp = temp->next;
            }

            /* UNIT TEST FOR SPLITTING */
            assert(temp_nblocks_after == (temp_nblocks_before + 1));
        }
    }

    /* Could not find free block, so grow heap */
    if (next == NULL) {
        next = grow_heap(last, size);
    }

    /* Could not find free block or grow heap, so just return NULL */
    if (next == NULL) {
        return NULL;
    }
    
    /* Mark block as in use */
    next->free = false;

    /* Increase counter */
    malloc_counter++;
    
    //debug_print();
    
    /* Return data address associated with block */
    return BLOCK_DATA(next);
    

}

/* Reclaim space -------------------------------------------------------------*/

void free(void *ptr) {
    if (ptr == NULL) {
        return;
    }
    char buffer[1<<10];
    sprintf(buffer,"here");

    /* Make block as free */
    struct block *curr = BLOCK_HEADER(ptr);
    curr->free = true;

    /* Find number of blocks before coalescing */
    int temp_nblocks_before = 0;
    struct block *temp = FreeList;
    while (temp) {
        temp_nblocks_before++;
        temp = temp->next;
    }
 

    /* TODO: Coalesce free blocks? */
    struct block *tmp_curr = FreeList;
    struct block *next_block;
    if(tmp_curr){
        while(tmp_curr->next){
            next_block = tmp_curr->next;
            if(tmp_curr->free && next_block->free){ 
                //Size = size of each block, plus the size saved by removing a block
                tmp_curr->size = tmp_curr->size + next_block->size + sizeof(struct block);
                tmp_curr->next = next_block->next;
                coalesce_counter++;
                tmp_curr = FreeList;
            }else{
                tmp_curr = next_block;
            }
        }
        /* Find number of blocks after splitting */
        int temp_nblocks_after = 0;
        temp = FreeList;
        while (temp) {
            temp_nblocks_after++;
            temp = temp->next;
        }

    }

    /* Increase counter */
    free_counter++;
    //debug_print();
}


/* vim: set expandtab sts=4 sw=4 ts=8 ft=cpp: --------------------------------*/
