// htcache.h: Hash Tree Cache

#pragma once

#include <htcache/htpage.h>

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <math.h>

template <typename KeyType, typename ValueType>
class HTCache {
    typedef ValueType (*HandlerFunc)(const KeyType&);
    typedef std::vector<HTPage<KeyType, ValueType>> TableType;

private:	
    size_t	    	AddressLength;		// Length of virtual address
    size_t	    	PageSize;		// Number of entries per page
    EvictionPolicy  	Policy;		    	// Eviction policy
    HandlerFunc	    	Handler;	    	// Handler for misses

    size_t	    	Addresses;		// Number of addresses
    size_t	    	Pages;	   		// Number of pages
    size_t	    	VPNShift;	    	// VPN Bit Shift
    size_t	    	VPNMask;	    	// VPN Bit Mask
    size_t	    	OffsetMask;	    	// Offset Bit Mask


    std::mutex	    	Lock;	    		// Lock

public:
    TableType	    	PageTable;	    	// Page table
    size_t	    	Hits;	    		// Number of cache hits
    size_t	    	Misses;	    		// Number of cache misses
    HTCache(size_t addrlen, size_t pagesize, EvictionPolicy policy, HandlerFunc handler) {
	if (addrlen == 0 || pagesize == 0) {
		AddressLength = 0; 
		Addresses = 0; 
		PageSize = 0;
		Pages = 0;
		VPNShift = 0;
		VPNMask = 0 << VPNShift;

	} else {
		AddressLength = addrlen;
		Addresses = pow(2, AddressLength);
		PageSize = pagesize;
		Pages = Addresses / PageSize;
		VPNShift = log2(PageSize);
		VPNMask = (Pages - 1) << VPNShift;
	}

	// TODO: Determine Addresses, Pages, VPNShift, VPNMask, OffsetMask
	Policy = policy;
	Handler = handler;
	Misses = 0;
	Hits = 0;

	OffsetMask = (1 << VPNShift) - 1;
	
	// TODO: Initialize PageTable	
	for (int i = 0; i < int(Pages); i++) {
		HTPage<KeyType, ValueType> page(PageSize, Policy);
		PageTable.push_back(page);
	}		
    }

    /* GET VALUE FROM CACHE */
    ValueType	get(const KeyType &key) {


    	if (PageTable.empty()) {
    	    return Handler(key);
	}

	// TODO: Determine virtual address, VPN, offset
	size_t VirtualAddress = std::hash<KeyType>{}(key);
	VirtualAddress = VirtualAddress % Addresses;
	size_t VPN = (VirtualAddress & VPNMask) >> VPNShift;
	size_t offset = VirtualAddress & OffsetMask;


	/* TODO: Retrieve value from HTPage */
	// find appropriate page
	ValueType value;
	HTPage<KeyType, ValueType> *page = &PageTable[VPN];
	try {
		value = page->get(key, offset);
		std::lock_guard<std::mutex> guard(Lock);
		Hits += 1;
	} catch (const std::out_of_range& oor) {
		value = Handler(key);
		page->put(key, value, offset);
		std::lock_guard<std::mutex> guard(Lock);
		Misses += 1;
	}

    	return value;
    }

    /* INSERT VALUE AND KEY IN CACHE */
    void put(const KeyType &key, const ValueType &value) {


        if (PageTable.empty()) {
    	    return;
	}

	// TODO: Determine virtual address, VPN, offset
	size_t VirtualAddress = std::hash<KeyType>{}(key);
	VirtualAddress = VirtualAddress % Addresses;
	size_t VPN = (VirtualAddress & VPNMask) >> VPNShift;
	size_t offset = (VirtualAddress & OffsetMask);

	// TODO: Set key, value in HTPage
	HTPage<KeyType, ValueType> *page = &PageTable[VPN];
	page->put(key, value, offset); 
    }

    void stats(FILE *stream) {
	fprintf(stream, "Addresses : %lu\n"  , Addresses);
	fprintf(stream, "Pages     : %lu\n"  , Pages);
	fprintf(stream, "VPNShift  : %lu\n"  , VPNShift);
	fprintf(stream, "VPNMask   : 0x%lX\n", VPNMask);
	fprintf(stream, "OffsetMask: 0x%lX\n", OffsetMask);
    	fprintf(stream, "Hits      : %lu\n"  , Hits);
    	fprintf(stream, "Misses    : %lu\n"  , Misses);
    }

};
