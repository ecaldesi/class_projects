/* malloc.c: simple memory allocator -----------------------------------------*/

#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

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

/* TODO: Add counters for mallocs, frees, reuses, grows */

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
    /* TODO: Support other policies */

    return curr;
}

/* Grow heap -----------------------------------------------------------------*/

struct block *grow_heap(struct block *last, size_t size) {
    /* Request more space from OS */
    struct block *curr = (struct block *)sbrk(0);
    struct block *prev = (struct block *)sbrk(sizeof(struct block) + size);

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
    /* Align to multiple of 4 */
    size = ALIGN4(size);

    /* Handle 0 size */
    if (size == 0) {
        return NULL;
    }

    /* Look for free block */
    struct block *last = FreeList;
    struct block *next = find_free(&last, size);

    /* TODO: Split free block? */

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

    /* Return data address associated with block */
    return BLOCK_DATA(next);
}

/* Reclaim space -------------------------------------------------------------*/

void free(void *ptr) {
    if (ptr == NULL) {
        return;
    }

    /* Make block as free */
    struct block *curr = BLOCK_HEADER(ptr);
    assert(curr->free == 0);
    curr->free = true;


    /* TODO: Coalesce free blocks? */
    struct block *next_block = curr->next;
    if (next_block->free == true){
       size_t size = next_block->size + curr->size;
       curr->size = size;
       curr->next = next_block->next; 
    }
}




/* vim: set expandtab sts=4 sw=4 ts=8 ft=cpp: --------------------------------*/
