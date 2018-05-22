// fibonacci.cpp
#include <htcache/htcache.h>
#include <vector>
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <random>
#include <thread>
#include <iomanip>
uint64_t Data[100] = {0};

void fib_thread(HTCache <int64_t, uint64_t> *cache) {
    	for (size_t i = 3; i < 100; i++) {
		int64_t sum1 = cache->get(i-1);
		int64_t sum2 = cache->get(i-2);
		int64_t total = sum1+sum2;
		Data[i] = total;
        }
}

int main(int argc, char *argv[]){
	if (argc != 5) {
		fprintf(stderr, "Usage: %s AddressLength PageSize EvictionPolicy Threads\n", argv[0]);
		return EXIT_FAILURE;
	}

        size_t addrlen        = strtol(argv[1], NULL, 10);
        size_t pagesize       = strtol(argv[2], NULL, 10);
        EvictionPolicy policy = static_cast<EvictionPolicy>(strtol(argv[3], NULL, 10));
        size_t nthreads	      = strtol(argv[4], NULL, 10);
	std::thread t[nthreads];
	

	HTCache<int64_t, uint64_t> is_fib(addrlen, pagesize, policy, [](const int64_t &key){ return Data[key]; });
	Data[0] = 0;
	Data[1] = 1;

	for (int i = 0; i < int(nthreads); i++) {
		t[i] = std::thread(fib_thread, &is_fib);
	}

	for (int i = 0; i < int(nthreads); i++) {
		t[i].join();
	}

	for (int i = 0; i< 100; i++) {
		uint64_t value = is_fib.get(i);
		std::cout << std::setw(3) << "Fibonacci (" << std::setw(2) << i << ") = " << value << std::endl;
	}

	is_fib.stats(stdout);

	return 0;
}
