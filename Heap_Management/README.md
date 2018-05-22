CSE.30341.FA17: Project 04
==========================

This is the documentation for [Project 04] of [CSE.30341.FA17].

Members
-------

1. Elisabetta Caldesi (ecaldesi@nd.edu)
2. Anthony DiFalco (adifalco@nd.edu)
3. Owen Phelan (ophelan@nd.edu)

Design
------

> 1. You will need to implement splitting of free blocks:
>
>   - When should wesplit a block?
>	Whenever there is a block that has too much space for the amount of memory that we would need to allocate.
>   - How should wesplit a block?
> 	Change the size of the block to half its size, and then have the next_pointer pointer point to the new block created. After this, we will update the value of block->free to false. The new block should have free true, its size half of the size of the other block, and its next pointer should be pointing at the block pointed to by the next pointer of the other block.

> 2. You will need to implement coalescing of free blocks:
>
>   - When shouldwecoalescing block?
>	Whenever we want to allocate memory and none of the blocks contain as much space as we need.
>   - How shouldwecoalesce a block?
	Take the first block and increase its size by the sum of the sizes of the contiuous free
	blocks. Then, we will have the next pointer point to the next block that the last empty contiuous block
	was pointing to. Then we will set the value of free to false. 
Response.

> 3. You will need to implement Next Fit.
>
>   - What information doweneed to perform a Next Fit?
>	The last block that was checked with Next Fit because we apply First Fit by starting from
	the last block checker. If we get to the end of the free list without having found a block, we
	loop back to the beginning of the free list.
>   - How wouldweimplement Next Fit?
	We set a pointer to the last block and iterate through the free list untilwefind an empty block big
	enough. If we don't find a fit, we go back to the beginning of the list and we iterate through 
	until we reach the pointer that was pointing to the initial last block.
Response.

> 4. You will need to implement Best Fit.
>
>   - What information doweneed to perform a Best Fit?
>	The current block. We store a pointer that looks at the best block found so far.
>   - How wouldweimplement Best Fit?
	Assuming the best block has a size that's equal to the size required or a little bigger,
	iterate through the list and store the best block found in a pointer that points at it, if we
	don't find a better block return that pointer, if not, update the pointer to a better block.
Response.

> 5. You will need to implement Worst Fit.
>
>   - What information doweneed to perform a Worst Fit?
>	We would need to have a pointer to the current block, and a pointer to the worst block found
	so far. 
>   - How wouldweimplement Worst Fit?
	Worst fit would work exactly like best fit, except for the fact that the worst block is the block
	in the free list that has the biggest possible size. 
Response.

> 6. You will need to implement tracking of different information.
>
>   - What information willwewant to track?
>	We will keep track of:
	- 1. number of times we reuse an existing block
	- 2. number of times the user calls malloc successfully
	- 3. number of times the user calls free successfully
	- 4. number of times we request a new block	
	- 5. number of times we split a block
	- 6. number of times we coalesce blocks
	- 7. number blocks in free list
	- 8. total amount of memory requested
	- 9. maximum size of the heap

>   - How willweupdate these trackers?
>	To keep track of those counters above, we would have to increase them inside of our functions.
	- Malloc: 2, 5, 8 (by adding all of the sizes passed into the function)
	- Free: 3, 6, 1
	- grow_heap: 4
	Number of blocks: just iterate through free list and have a counter
	Maximum size of heap: sbrk(0)
>   - How willwereport these trackers when the program ends?
	We will create a print function and declare it as atexit so it prints out whenever the program
	exits. 
Response.

Demonstration
-------------

> Place a link to your demonstration slides on [Google Drive].
https://docs.google.com/presentation/d/1itUGcA8qFCBWHn2VkLoo3juoAyiW0KQSQFPXkNwcpiY/edit#slide=id.g2ae0bdc202_0_10
Errata
------

> Describe any known errors, bugs, or deviations from the requirements.

Extra Credit
------------

> Describe what extra credit (if any) thatweimplemented.
We implemented the extra credit for coalescing.

[Project 04]:       https://www3.nd.edu/~pbui/teaching/cse.30341.fa17/project04.html
[CSE.30341.FA17]:   https://www3.nd.edu/~pbui/teaching/cse.30341.fa17/
[Google Drive]:     https://drive.google.com
