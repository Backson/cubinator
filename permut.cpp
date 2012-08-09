#include "permut.h"

template<int length>
const uint64_t Perm<length>::FACTORIALS[] = {
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
Perm<length>::Perm() {
	for (int i = 0; i < length; i++)
		m_elements[i] = i;
}

template<int length>
Perm<length>::Perm(uint64_t id) {
	bool visited[length];
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
		
		m_elements[i] = index;
		id %= FACTORIALS[length - i - 1];
		visited[index] = true;
	}
}

template<int length>
Perm<length>::Perm(std::initializer_list<int> elements) {
	int i = 0;
	for (auto iter = elements.begin(); iter != elements.end(); iter++, i++)
		m_elements[i] = *iter;
}

template<int length>
Perm<length>::Perm(int elements[length]) {
	for (int i = 0; i < length; i++)
		m_elements[i] = elements[i];
}

template<int length>
uint64_t Perm<length>::id() const {
	uint64_t result = 0;
	
	// save which numbers were already 
	uint8_t visited[length];
	memset(visited, 0, length * sizeof (uint8_t));
	
	// iterate over the elements of the permutation
	for (int i = 0; i < length; i++) {
		
		// we want this number.
		auto element = m_elements[i];
		
		// which place is the element in the list of unused elements?
		int sum = 0;
		for (int index = 0; index < element; index++) {
			if (!visited[index]) {
				sum++;
			}
		}
		
		result += FACTORIALS[length - i - 1] * sum;
		visited[m_elements[i]] |= 1;
	}
	
	return result;
}

template<int length>
Perm<length> Perm<length>::operator+(const Perm& that) const {
	Perm result;
	
	for (int i = 0; i < length; i++) {
		result.m_elements[i] = this->m_elements[that.m_elements[i]];
	}
	
	return result;
}

template<int length>
Perm<length> Perm<length>::operator*(int exponent) const {
	Perm<length> result;
	
	if (exponent < 0) {
		result = result.inverse();
		exponent *= -1;
	}
	
	while(exponent > 0) {
		result *= *this;
		exponent--;
	}
	
	return result;
}

template<int length>
void Perm<length>::swap(int a, int b) {
	int triangle = m_elements[a];
	m_elements[a] = m_elements[b];
	m_elements[b] = triangle;
}

template<int length>
Perm<length> Perm<length>::inverse() const {
	Perm<length> result;
	
	for (int i = 0; i < length; i++) {
		result.m_elements[this->m_elements[i]] = i;
	}
	
	return result;
}

template<int length>
int Perm<length>::parity() {
	int inversion_count = 0;
	
	for (int j = 0; j < length; j++) {
		for (int i = 0; i < j; i++) {
			if (m_elements[i] > m_elements[j])
				inversion_count++;
		}
	}
	
	return 1 - (inversion_count % 2) * 2;
}

