#include "HashSet.h"

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
	delete[] slots;
}

// double table size, recreate integer hash function based on num of buckets
// re-insert every item
void HashSet::rehash() {
	int index;
	int input;
	this->nslots = 2*nslots;
	std::string** temp = new std::string*[nslots]();
	delete intfn;
	// get new integer hash function
	this->intfn = new SquareRootHash(0, nslots);
	// re-inserts every item in new table
	for(int i = 0; i < nslots/2; i++) {
		if(slots[i] != NULL) {
			input = strfn->hash(*slots[i]);
			index = intfn->hash(input);
			// collision check
			while(temp[index] != NULL) 
				index = intfn->hash(index+1);
			temp[index] = slots[i];
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
		if(nslots == nitems) {
		       	rehash();
			index = intfn->hash(input);
		}
		// probing
		while(slots[index] != NULL)
			index = intfn->hash(input+1);
		*slots[index] = value;
	}

}

bool HashSet::lookup(const std::string& value) const {
	uint64_t input = strfn->hash(value);
	uint64_t index = intfn->hash(input);
	while(slots[index] != NULL) {
		if(*slots[index] == value)
			return true;
		index = intfn->hash(input+1);
	}
	return false;
}

