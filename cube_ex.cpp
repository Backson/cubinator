#include "cube_ex.h"

const ExtendedCube ExtendedCube::TURN_MIDDLE(
	{ 0, 1, 2, 3, 8, 5, 4, 7,10, 9, 6,11}, {0,1,2,3,4,5,6,7},
	{ 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0}, {0,0,0,0,0,0,0,0},
	{3,0,2,5,4,1});
const ExtendedCube ExtendedCube::TURN_EQUATOR(
	{ 3, 0, 1, 2, 4, 5, 6, 7, 8, 9,10,11}, {0,1,2,3,4,5,6,7},
	{ 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}, {0,0,0,0,0,0,0,0},
	{0,4,1,2,3,5});
const ExtendedCube ExtendedCube::TURN_STANDING(
	{ 0, 1, 2, 3, 4, 9, 6, 5, 8,11,10, 7}, {0,1,2,3,4,5,6,7},
	{ 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1}, {0,0,0,0,0,0,0,0},
	{4,1,0,3,5,2});

const ExtendedCube ExtendedCube::TURN_X =
	ExtendedCube(TURN_RIGHT_INITIALIZER, Perm<6>())
	- ExtendedCube(TURN_LEFT_INITIALIZER, Perm<6>())
	- ExtendedCube::TURN_MIDDLE;
const ExtendedCube ExtendedCube::TURN_Y =
	ExtendedCube(TURN_UP_INITIALIZER, Perm<6>())
	- ExtendedCube(TURN_DOWN_INITIALIZER, Perm<6>())
	- ExtendedCube::TURN_EQUATOR;
const ExtendedCube ExtendedCube::TURN_Z =
	ExtendedCube(TURN_FRONT_INITIALIZER, Perm<6>())
	- ExtendedCube(TURN_BACK_INITIALIZER, Perm<6>())
	+ ExtendedCube::TURN_STANDING;

ExtendedCube::ExtendedCube(std::initializer_list<int> edges, std::initializer_list<int> corners,
		std::initializer_list<int> edge_orients, std::initializer_list<int> corner_orients,
		const Perm<6>& middles)
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

ExtendedCube ExtendedCube::operator-() const {
	return this->inverse();
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

bool ExtendedCube::operator==(const ExtendedCube& that) {
	if (Cube::operator!=(that)) return false;
	if (this->m_middles!=that.m_middles) return false;
	return true;
}

bool ExtendedCube::operator!=(const ExtendedCube& that) {
	return !operator==(that);
}

ExtendedCube ExtendedCube::inverse() const {
	Cube conv(*this);
	ExtendedCube result(conv.inverse(), this->m_middles.inverse());
	return result;
}

ExtendedCube ExtendedCube::parse(const char* msg) {
	ExtendedCube result;
	
	char c;
	
	ExtendedCube last = ExtendedCube(TURN_IDENTITY);
	
	while (c = *msg++) {
		switch (c) {
		case 'R': last = TURN_RIGHT; break;
		case 'L': last = TURN_LEFT; break;
		case 'U': last = TURN_UP; break;
		case 'D': last = TURN_DOWN; break;
		case 'F': last = TURN_FRONT; break;
		case 'B': last = TURN_BACK; break;
		case 'M': last = TURN_MIDDLE; break;
		case 'E': last = TURN_EQUATOR; break;
		case 'S': last = TURN_STANDING; break;
		case 'x': last = TURN_X; break;
		case 'y': last = TURN_Y; break;
		case 'z': last = TURN_Z; break;
		case 'r': last = TURN_RIGHT - TURN_MIDDLE; break;
		case 'l': last = TURN_LEFT + TURN_MIDDLE; break;
		case 'u': last = TURN_UP - TURN_EQUATOR; break;
		case 'd': last = TURN_DOWN + TURN_EQUATOR; break;
		case 'f': last = TURN_FRONT + TURN_STANDING; break;
		case 'b': last = TURN_BACK - TURN_STANDING; break;
		case ' ': continue;
		default: throw "unknown character";
		}
		if (msg[0] == '2') {
			result += last * 2;
			msg++;
		} else if (msg[0] == '\'') {
			result -= last;
			msg++;
		} else {
			result += last;
		}
	}
	
	return result;
}