// test.cpp

#include <htcache/htcache.h>

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <random>
#include <thread>

int main(void){
	HTCache<int, int> is_odd(4, 4, EVICT_FIFO, [](const int &key){ return key % 2; });

	for( int i = 0 ; i < 20 ; i++){
		is_odd.get(i);
	}

	is_odd.stats(stdout);
	return 0;
}	
