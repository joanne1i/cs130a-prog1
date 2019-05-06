#include "HashSet.h"
#include <iostream>

HashSet::HashSet() {
	this->nitems = 0;
	this->nslots = 1000;
	this->intfn = new SquareRootHash(0, nslots);
	this->strfn = new JenkinsHash();
	this->strfn2 = NULL;
	this->slots = new std::string*[nslots]();
}

HashSet::~HashSet() {
	for(int i = 0; i < nslots; i++) {
		delete slots[i];
	}
	delete[] slots;
	delete strfn;
	delete intfn;
}

// double table size, recreate integer hash function based 
// on num of buckets re-insert every item
void HashSet::rehash() {
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
	uint64_t index = intfn->hash(input);
	if(lookup(value) == false) {
		nitems++;
		if(nitems >= nslots) {
			rehash();
			index = intfn->hash(input);
		}
		// linear probing
		while(slots[index%nslots] != NULL)
			index++;
		slots[index%nslots] = new std::string(value);
	}
}

bool HashSet::lookup(const std::string& value) const {
	uint64_t input = strfn->hash(value);
	uint64_t index = intfn->hash(input);
	while(slots[index%nslots] != NULL) {
		if(*slots[index%nslots] == value)
			return true;
		index++;
	}
	return false;
}

