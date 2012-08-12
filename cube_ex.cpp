#include "cube_ex.h"

const ExtendedCube::TURN_RIGHT(Cube::TURN_RIGHT);
const ExtendedCube::TURN_MIDDLE(Cube::TURN_RIGHT-Cube::TURN_LEFT, {3,0,2,5,4,1});

ExtendedCube::ExtendedCube(std::initializer_list<int> edges, std::initializer_list<int> corners,
		std::initializer_list<int> edge_orients, std::initializer_list<int> corner_orients,
		std::initializer_list<int> middles)
		: Cube(edges, corners, edge_orients, corner_orients), m_middles(middles) {
	// NOTHING
}

ExtendedCube::ExtendedCube(const Cube& cube)
		: Cube(cube) {
	// NOTHING
}

ExtendedCube::ExtendedCube(const Cube& cube, Perm<6> middles)
		: Cube(cube), m_middles(middles) {
	// NOTHING
}

ExtendedCube& ExtendedCube::operator=(const ExtendedCube& that) {
	Cube::operator=(that);
	m_middles = that.m_middles;
	return *this;
}

ExtendedCube ExtendedCube::operator+(const ExtendedCube& that) const {
	ExtendedCube result(*this);
	result.operator+=(that);
	return result;
}
ExtendedCube& ExtendedCube::operator+=(const ExtendedCube& that) {
	Cube::operator+=(that);
	m_middles += that.m_middles;
	return *this;
}

ExtendedCube ExtendedCube::operator-(const ExtendedCube& that) const {
	return operator+(that.inverse());
}
ExtendedCube& ExtendedCube::operator-=(const ExtendedCube& that) {
	operator+=(that.inverse());
	return *this;
}

ExtendedCube ExtendedCube::operator*(int exponent) const {
	ExtendedCube result(*this);
	result.operator*=(exponent);
	return result;
}
ExtendedCube& ExtendedCube::operator*=(int exponent) {
	Cube::operator*=(exponent);
	m_middles *= exponent;
	return *this;
}

ExtendedCube ExtendedCube::inverse() const {
	Cube conv(*this);
	ExtendedCube result(result.inverse(), this->m_middles.inverse());
	return result;
}
