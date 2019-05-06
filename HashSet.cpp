#include "HashSet.h"
#include <iostream>

HashSet::HashSet() {
	this->nitems = 0;
	// will have to resize when nitems == nslots
	this->nslots = 1000;
	this->intfn = new SquareRootHash(0, nslots);
	this->strfn = new JenkinsHash();
	this->strfn2 = new PearsonHash();
	this->slots = new std::string*[nslots]();
}

HashSet::~HashSet() {
	for(int i = 0; i < nslots; i++) {
		delete slots[i];
	}
	delete[] slots;
}

// double table size, recreate integer hash function based on num of buckets
// re-insert every item
void HashSet::rehash() {
	std::cout << "rehash is called" << std::endl;
	uint64_t index;
	uint64_t input;
	this->nslots = 2*nslots;
	std::string** temp = new std::string*[nslots]();
	delete this->intfn;
	// get new integer hash function
	this->intfn = new SquareRootHash(0, nslots);
	// re-inserts every item in new table
	for(int i = 0; i < nslots/2; i++) {
		if(slots[i] != NULL) {
			input = strfn->hash(*slots[i]);
			index = intfn->hash(input);
			// collision check
			while(temp[index%nslots] != NULL) 
				index++;
			temp[index%nslots] = slots[i];
		}
	}
	delete[] slots;
	slots = temp;

}

void HashSet::insert(const std::string& value) {
	uint64_t input = strfn->hash(value);
	// get the index for the value
	uint64_t index = intfn->hash(input);
	if(lookup(value) == false) {
		nitems++;
		if(nitems >= nslots) {
		//	std::cout << "rehash is called in insert" << std::endl;
			rehash();
			index = intfn->hash(input);
		//	std::cout << "rehahs is completed" << std::endl;
		}
		//std::cout << "lookup completed" << std::endl;
		// probing
		
		while(slots[index%nslots] != NULL)
			index++;
		//std::cout << "seg fault at hashset insert" << std::endl;
		slots[index%nslots] = new std::string(value);
		//std::cout << "it's the slots thing" << std::endl;
	}
}

bool HashSet::lookup(const std::string& value) const {
	//std::cout << "hash set lookup is called" << std::endl;
	uint64_t input = strfn->hash(value);
	uint64_t index = intfn->hash(input);
	while(slots[index%nslots] != NULL) {
		if(*slots[index%nslots] == value)
			return true;
		index++;
	}
	//std::cout << "lookup complete" << std::endl;
	return false;
}

