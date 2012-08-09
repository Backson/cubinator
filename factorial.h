#ifndef factorial_h
#define factorial_h

#include "permut.h"

#include <string.h>

static const uint64_t FACTORIALS[] = {
	1l, // 0
	1l,
	2l,
	6l,
	24l,
	120l, // 5
	720l,
	5040l,
	40320l,
	362880l,
	3628800l, // 10
	39916800l,
	479001600l,
	6227020800l,
	87178291200l,
	1307674368000l, // 15
	20922789888000l,
	355687428096000l,
	6402373705728000l,
	121645100408832000l,
	2432902008176640000l // 20
};

template<int length>
uint64_t to_id(const Perm<length>& perm) {
	uint64_t result = 0;
	
	// save which numbers were already 
	uint8_t visited[length];
	memset(visited, 0, length * sizeof (uint8_t));
	
	// iterate over the elements of the permutation
	for (int i = 0; i < length; i++) {
		
		// we want this number.
		auto element = perm.element(i);
		
		// which place is the element in the list of unused elements?
		int sum = 0;
		for (int index = 0; index < element; index++) {
			if (!visited[index]) {
				sum++;
			}
		}
		
		result += FACTORIALS[length - i - 1] * sum;
		visited[perm.element(i)] |= 1;
	}
	
	return result;
}

template<int length>
Perm<length> from_id(uint64_t id) {
	int table[length];
	
	uint8_t visited[length];
	memset(visited, 0, length * sizeof (uint8_t));
	
	for (int i = 0; i < length; i++) {
		
		int location = id / FACTORIALS[length - i - 1];
		
		int index = 0;
		for (int counter = 0; index < length; index++) {
			if (!visited[index]) {
				if (counter == location)
					break;
				else
					counter++;
			}
		}
		
		table[i] = index;
		id %= FACTORIALS[length - i - 1];
		visited[index] |= 1;
	}
	
	return Perm<length>(table);
}

#endif // factorial_h
