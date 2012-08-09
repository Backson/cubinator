#ifndef permut_h
#define permut_h

#include <cstdint>
#include <initializer_list>

template<int length>
class Perm {
public:
	
	Perm();
	Perm(const Perm& that) { this->operator=(that); }
	/*Perm(uint64_t id);*/
	Perm(std::initializer_list<int> elements);
	Perm(uint8_t elements[length]);
	Perm(int elements[length]);
	~Perm() = default;
	
	// set this permutation to another one
	Perm& operator=(const Perm&);
	
	// concatenate this permutation with another one
	Perm operator+(const Perm&) const;
	// careful, this multiplies from the LEFT side, like mathematical operators
	Perm& operator+=(const Perm&);
	
	// concatenate this permutation with another one
	Perm operator*(int exponent) const;
	Perm& operator*=(int exponent);
	
	// swap two elements
	void swap(int,int);
	
	// which Permutation reverses this one?
	Perm inverse() const;
	
	// get the parity of the permutation
	/*int parity();*/
	
	// onto which element is element i projected by this permutation?
	int element(int i) const { return m_elements[i]; }
	
	// which id does this permutation have?
	/*uint64_t id() { return m_id; }*/
	
private:
	/*uint64_t m_id;*/
	int m_elements[length];
};

template<int length>
Perm<length>::Perm() {
	for (int i = 0; i < length; i++)
		m_elements[i] = i;
}
/*
template<int length>
Perm<length>::Perm(uint64_t id) {
	throw 0;
}
*/
template<int length>
Perm<length>::Perm(std::initializer_list<int> elements) {
	int i = 0;
	for (auto iter = elements.begin(); iter != elements.end(); iter++, i++)
		m_elements[i] = *iter;
}

template<int length>
Perm<length>::Perm(uint8_t elements[length]) {
	for (int i = 0; i < length; i++)
		m_elements[i] = elements[i];
}

template<int length>
Perm<length>::Perm(int elements[length]) {
	for (int i = 0; i < length; i++)
		m_elements[i] = elements[i];
}

template<int length>
Perm<length>& Perm<length>::operator=(const Perm& that) {
	for (int i = 0; i < length; i++)
		m_elements[i] = that.m_elements[i];
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
Perm<length>& Perm<length>::operator+=(const Perm& that) {
	operator=((*this) + that);
	return *this;
}

/* a *= b      a = b * a */

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
Perm<length>& Perm<length>::operator*=(int exponent) {
	operator=(*this * exponent);
	return *this;
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
/*
template<int length>
int Perm<length>::parity() {
	
}
*/
#endif // permut_h
