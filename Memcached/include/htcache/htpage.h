// htpage.h: Hash Tree Page

#pragma once

#include <iostream>
#include <cstdlib>
#include <mutex>
#include <queue>
#include <random>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <time.h>
#include <stdexcept>

int myrand (int i) {return rand() %i;}
enum EvictionPolicy {
    EVICT_FIFO,
    EVICT_RANDOM,
    EVICT_LRU,
    EVICT_CLOCK,
};

template <typename KeyType, typename ValueType>
struct HTPageEntry {
    KeyType	Key;
    ValueType	Value;
    bool valid = true;
    bool occupied = false;
    // TODO: Add bookkeeping for eviction
    int indicator = 0;
};

template <typename KeyType, typename ValueType>
class HTPage {
    typedef std::vector<HTPageEntry<KeyType, ValueType>> EntriesType;

private:
    EvictionPolicy	Policy;	    // Eviction policy
    mutable std::mutex	Lock;	    // Lock
    size_t		PageSize;   // Number of Entries	
    // TODO: Add bookkeeping for eviction
    size_t		clk_index;  // clock head

    size_t evict_fifo(size_t offset) {
	// TODO: Implement FIFO eviction policy
    	int evicted_index = 0;
    	int tmp_index = 0;
    	int max_indicator = 0;

	// find highest indicator and evict that entry	
	for( auto it = Entries.begin() ; it != Entries.end() ; it++){
		if(it->indicator > max_indicator){
			max_indicator = it->indicator;
			evicted_index = tmp_index;
		}
		tmp_index++;
	}
	Entries[evicted_index].indicator = 0;

	return evicted_index;
    }

    size_t evict_random(size_t offset) {
    	// TODO: Implement random eviction policy
    	return rand()%PageSize;
    }

    size_t evict_lru(size_t offset) {
    	int evicted_index = 0;
    	int tmp_index = 0;
    	int max_indicator = 0;

	// find highest indicator and evict that entry	
	for( auto it = Entries.begin() ; it != Entries.end() ; it++){
		if(it->indicator > max_indicator){
			max_indicator = it->indicator;
			evicted_index = tmp_index;
		}
		tmp_index++;
	}
	Entries[evicted_index].indicator = 0;

	return evicted_index;
    }

    size_t evict_clock(size_t offset) {
    	// TODO: Implement clock eviction policy
	size_t it = clk_index + 1;
	if(it == PageSize){
		it = 0;
	}
	
	while (true) {
		if (Entries[it].valid == true) {
			Entries[it].valid = false;
			it++;
			if(it == PageSize){
				it = 0;
			}
		} else {
			Entries[it].valid = true;
			break;
		}
	}
	clk_index = it;
    	return it;
    }


public:
    EntriesType Entries;

    // CONSTRUCTOR
    HTPage(size_t n, EvictionPolicy p) {
    	// TODO: Initialize Entries	
	srand(time(NULL));
	PageSize = n;
	Policy = p;
	clk_index = 0;
	
	// create entry with right book keeping
	for(int i = 0 ; i < int(n); i++){
		HTPageEntry <KeyType, ValueType> entry;
		Entries.push_back(entry);
	}

    }
    
    // COPY CONSTRUCTOR 
    HTPage(const HTPage<KeyType, ValueType>& other) {
    	// TODO: Copy instance variables
	other.Lock.lock();	
	Entries = other.Entries;    // Individual entries
	clk_index = other.clk_index;
	Policy = other.Policy;	    // Eviction policy
	PageSize = other.PageSize;
	other.Lock.unlock();
    }

    ValueType get(const KeyType &key, size_t offset) {
	std::lock_guard<std::mutex> guard(Lock);
 	// TODO: Use linear probing to locate key
	KeyType currKey;
	size_t i = offset;
	ValueType value;

	while(true) {
		if(Entries[i].occupied==false){
			throw std::out_of_range ("Range");
			break;
		} else {
			currKey = Entries[i].Key;
			if(currKey == key) {
				value = Entries[i].Value;
				break;
			}
		}
		i++;
		if(i == PageSize){
			i = 0;
		}
		if(i == offset){
			throw std::out_of_range ("Range");
			break;
		}
    	}
	// update bookkeeping
	get_update(i);
	return value;
	}	

    void put(const KeyType &key, const ValueType &value, size_t offset) {
	HTPageEntry<KeyType, ValueType> *entry_ptr = &Entries[offset];
	std::lock_guard<std::mutex> guard(Lock);
    	// TODO: Use linear probing to locate key
	size_t i = offset;
	while(true) {
		if(!Entries[i].occupied){
			entry_ptr = &Entries[i];
			break;
		} else if (Entries[i].Key == key) {
			entry_ptr = &Entries[i];
			break;	
		}
		i = i+1;
		if (i == PageSize) {
			i = 0;
		} 
		if (i == offset) {
			// TODO: Evict an entry if HTPage is full
			if (Policy == EVICT_FIFO) {
				i = evict_fifo(offset);
				entry_ptr = &Entries[i];
			} else if (Policy == EVICT_CLOCK) {
				i = evict_clock(offset);
				entry_ptr = &Entries[i];
			} else if (Policy == EVICT_LRU) {
				i = evict_lru(offset);
				entry_ptr = &Entries[i];
			} else if (Policy == EVICT_RANDOM) {
				i = evict_random(offset);
				entry_ptr = &Entries[i];
			}
			break;
		}
	}
	
	entry_ptr->Value = value;
	entry_ptr->Key = key;
	entry_ptr->occupied = true;
	entry_ptr->indicator = 0;
	// Update bookkeeping
	put_update();		
	
    }

    int get_update(size_t offset){
	// LRU
	if(Policy==EVICT_LRU){
		// update indicator of most recently used entry to 0
		Entries[offset].indicator = 0;
		// update all of the other entries' indicators
		put_update();

		return 0;
	}

	// Clock
	else if (Policy==EVICT_CLOCK){
		// Locate in circular list
		clk_index = offset;
		return 0;
	}

	if (Policy==EVICT_FIFO || Policy==EVICT_RANDOM) return 0;
        return -1;
    }

    int put_update(){
	// FIFO & Random
	if(Policy==EVICT_FIFO || Policy==EVICT_LRU){		
		for( auto it = Entries.begin() ; it != Entries.end() ; it++){
			if(it->occupied){
				// increase the indicators of all of the entries that are full
				it->indicator++;
			}
		}
		return 0;
	}
	// Clock
	else if (Policy==EVICT_CLOCK || Policy==EVICT_RANDOM){
		return 0;
	}

        return -1;
    
    }

};
