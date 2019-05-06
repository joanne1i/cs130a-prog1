#include "HashSet.h"

HashSet::HashSet() {
	this->nitems = 0;
	// will have to resize when nitems == nslots
	this->nslots = 100;
	this->intfn = new SquareRootHash(uint64_t i, nslots);
	this->strfn = new JenkinsHash(const std::string& input);
	this->strfn2 = new PearsonsHash(const std::string& input);
	this->slots = new std::string*[nslots];
}
// double table size, recreate integer hash function based on num of buckets
// re-insert every item
void HashSet::rehash() {
	this->nslots = 2*nslots;
	this->nitems = 0;
	std::string** temp = new std::string*[nslots];
	// get new integer hash function
	this->intfn = new SquareRootHash(uint64_t i, nslots);
	// re-inserts every item in new table
	for(int i = 0; i < slots.size(); i++) {
		while(slots[i] != NULL) {
			temp[index] = insert(slots[i]);
		}
	}
	delete[] slots;
	slots = temp;

}

void HashSet::insert(const std::string& value) {
	uint64_t input = strfn.hash(value);
	// get the index for the value
	uint64_t index = intfn.hash(input);
	if(lookup(value) == false) {
		nitems++;
		if(nslots == nitems) rehash();
		// linear probing
		while(slots[index] != NULL)
			index = intfn.hash(input+1);
		slots[index] = value;
	}

}

bool HashSet::lookup(const std::string& value) const {



}

