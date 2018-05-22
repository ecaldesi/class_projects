# Memcached
By Elisabetta Caldesi, Anthony DiFalco, Owen Phelan

Build a simplified implementation of memcached, which is an in-memory key-value store that is used for quick access to 
frequently used computations (i.e. a cache). <br />

HTCache uses a modified form of a hashed array tree, which combines facets of hash tables with binary trees. Rather than using 
the result of the hash function to map to a specific bucket, we will use the hash function to convert a key into a virtual 
address that we can then use to navigate our hashed array tree in a manner similar to how we translate virtual address to 
physical addresses with page tables. <br />

Inside HTCache, we have a PageTable, which is actually just a std::vector of HTPages. Likewise, each HTPage itself contains 
a std::vector of HTPageEntry objects which at the very least keep track of the key and value pair associated with the HTPageEntry. <br />

We also utilized this program to create a fibonacci implementation. <br />
