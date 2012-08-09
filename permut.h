#ifndef permut_h
#define permut_h

#include <cstdint>
#include <initializer_list>

template<int length>
class Perm {
public:
	
	// standard constructor
	Perm();
	// copy constructor
	Perm(const Perm& that) { this->operator=(that); }
	// initialize from a number from 0 to (length!)-1. The mapping is arbitrary
	Perm(uint64_t id);
	// initialize from a list of ints. different versions
	Perm(std::initializer_list<int> elements);
	Perm(int elements[length]);
	
	// which id does this permutation have?
	uint64_t id() const;
	
	// we don't dynamically allocate memory, so no destructor needed
	~Perm() = default;
	
	// set this permutation to another one
	inline Perm& operator=(const Perm& that) { memcpy(this, &that, sizeof (Perm<length>)); return *this; };
	
	// comparison operator
	inline bool operator==(const Perm& that) const { return memcmp(this, &that, sizeof (Perm<length>)) == 0; };
	inline bool operator!=(const Perm& that) const { return memcmp(this, &that, sizeof (Perm<length>)) != 0; };
	
	// concatenate this permutation with another one
	Perm operator+(const Perm&) const;
	
	// concatenate this permutation with another one
	Perm operator*(int exponent) const;
	
	// shortcuts for operator assignments
	inline Perm& operator+=(const Perm& that) { operator=((*this) + that); return *this; };
	inline Perm& operator*=(int exponent) { operator=((*this) * exponent); return *this; };
	
	// swap two elements
	void swap(int,int);
	
	// which Permutation reverses this one?
	Perm inverse() const;
	
	// get the parity of the permutation
	int parity();
	
	// onto which element is element i projected by this permutation?
	int element(int i) const { return m_elements[i]; }
	
private:
	int m_elements[length];
	
	// save the factorials up to 20! for convinience
	static const uint64_t FACTORIALS[];
};

#include "permut.cpp"

#endif // permut_h
