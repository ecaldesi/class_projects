# Heap Mangagement, Implementation of malloc and free
By Elisabetta Caldesi, Anthony DiFalco, Owen Phelan

FEATURES
-----------

- Implement splitting and coalescing of free blocks.
- Implement additional heap management strategies: Next Fit, Best Fit, and Worst Fit (First Fit has already been implemented for you).
- Use C's #if directive to select different blocks of code at compile time.
- Add counters for tracking of the following events, which should be displayed when the program exits:
  - Number of times the user calls malloc successfully
  - Number of times the user calls free successfully
  - Number of times we reuse an existing block
  - Number of times we request a new block
  - Number of times we split a block
  - Number of times we coalesce blocks
  - Number blocks in free list
  - Total amount of memory requested
  - Maximum size of the heap
- Create tests to check the correctness of your implementations.
- Create benchmarks to analyze the metrics above for the different strategies.sdfd
