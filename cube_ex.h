#ifndef cube_ex_h
#define cube_ex_h

#include "cube.h"

class ExtendedCube : public Cube {
public:
	ExtendedCube() = default;;
	ExtendedCube(const ExtendedCube& cube) { *this=cube; };
	ExtendedCube(std::initializer_list<int> edges, std::initializer_list<int> corners,
		std::initializer_list<int> edge_orients, std::initializer_list<int> corner_orients,
		std::initializer_list<int> middles);
	ExtendedCube(const Cube& cube);
	ExtendedCube(const Cube& cube, Perm<6> middles);
	~ExtendedCube() = default;
	
	
	ExtendedCube& operator=(const ExtendedCube&);
	
	ExtendedCube operator+(const ExtendedCube&) const;
	ExtendedCube& operator+=(const ExtendedCube&);
	
	ExtendedCube operator-(const ExtendedCube&) const;
	ExtendedCube& operator-=(const ExtendedCube&);
	
	ExtendedCube operator*(int exponent) const;
	ExtendedCube& operator*=(int exponent);
	
	ExtendedCube inverse() const;
	
	const Perm<6>& middles() const { return m_middles; }
	
	static ExtendedCube parse(const char*);
private:
	Perm<6> m_middles;
};

#endif //cube_ex_h