#include "BloomFilter.h"
#include <iostream>
BloomFilter::BloomFilter(int k, int m, std::string strfn, std::string intfn) {
	this->k = k; // num of hash functions
	this->m = m; // num of bits/table size
	this->intfns = new IntegerHash*[k]();
	
	// strfn hash functions
	if(strfn == "jenkins") 
		this->strfn = new JenkinsHash();
	else
		this->strfn = new PearsonHash();
	
	// intfns hash functions
	for(int j = 0; j < k; j++) {
		if(intfn == "squareroot") {
			this->intfns[j] = new SquareRootHash(j, m);
		}
		else if(intfn == "division") {
			this->intfns[j] = new DivisionHash(j,m);
		}
		else {
			this->intfns[j] = new ReciprocalHash(j,m);
		}
	}

	// all bits are set to zero
	this->bits = new uint64_t[(m+63)/64]();
}

BloomFilter::~BloomFilter() {
	delete[] bits;
	for(int i = 0; i < k; i++) {
		delete intfns[i];
	}
	delete[] intfns;
	delete strfn;
}

// run item through all the hash functions
// use results as indices into the bits array and set those
// bits as one
void BloomFilter::insert(const std::string& value) {
	uint64_t input = strfn->hash(value);
	uint64_t index;
	if(!lookup(value)) {
		// loops through hash functions 
		for(int i = 0; i < k; i++) {
			index = intfns[i]->hash(input);
			// bit manipulation
		//	std::cout << "seg fault at insert" << std::endl;
			bits[index/64] |= (uint64_t(1) << (index%64));
		}
	}
}

// run item through all the hash functions
// use results to index into the bits and report true if all
// referenced bits are one
bool BloomFilter::lookup(const std::string& value) const {
	uint64_t input = strfn->hash(value);
	uint64_t index;
	for(int i = 0; i < k; i++) {
		index = intfns[i]->hash(input);
	//	std::cout << "seg fault at lookup" << std::endl;
		if((bits[index/64] & (uint64_t(1) << (index%64))) == 0)
			return false;
	}
	return true;
}


