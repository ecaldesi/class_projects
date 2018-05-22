// unit_test.cpp

#include <htcache/htcache.h>

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <random>
#include <thread>

int main(int argc, char *argv[]){
	if (argc != 5) {
		fprintf(stderr, "Usage: %s AddressLength PageSize EvictionPolicy Threads\n", argv[0]);
		return EXIT_FAILURE;
	}

        size_t addrlen        = strtol(argv[1], NULL, 10);
        size_t pagesize       = strtol(argv[2], NULL, 10);
        EvictionPolicy policy = static_cast<EvictionPolicy>(strtol(argv[3], NULL, 10));
        size_t nthreads	      = strtol(argv[4], NULL, 10);
	
	HTCache<int64_t, int64_t> is_odd(addrlen, pagesize, policy, [](const int64_t &key){ return key % 2; });
	
	// testing htcache.h
	for (size_t i = 1; i< 5; i++) {
		is_odd.get(i);
		assert(is_odd.Misses == i);
		assert(is_odd.Hits == i-1);
		is_odd.get(i);
		assert(is_odd.Misses == i);
		assert(is_odd.Hits == i);	
	}
	for (size_t i = 5; i< 10; i++) {
		int64_t value = i%2;
		is_odd.put(i,value);
		int64_t returned = is_odd.get(i);
		assert(value == returned);
	}
	
	// testing htpage.h
	HTPage<int64_t, int64_t> page(pagesize, policy);

	for (size_t i = 0; i <100; i++) {
		size_t j = i% pagesize;
		page.put(i,i,j);
	}
	for (size_t i = 0; i <100; i++) {
		size_t j = i% pagesize;
		std::cout << page.get(i,j) << std::endl;
	}
	return 1;	
}
