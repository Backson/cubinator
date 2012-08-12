#include "cube.h"

#include <string.h>

const Cube Cube::TURN_IDENTITY;

const Cube Cube::TURN_RIGHT(
	{ 0, 5, 9, 3, 4, 2, 6, 7, 8, 1,10,11}, {0,2,6,3,4,1,5,7},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0,1,2,0,0,2,1,0});
const Cube Cube::TURN_LEFT(
	{11, 1, 2, 7, 4, 5, 6, 0, 8, 9,10, 3}, {4,1,2,0,7,5,6,3},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {2,0,0,1,1,0,0,2});

const Cube Cube::TURN_UP(
	{ 0, 1, 2, 3, 4, 5, 6, 7, 9,10,11, 8}, {0,1,2,3,5,6,7,4},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0,0,0,0,0,0,0,0});
const Cube Cube::TURN_DOWN(
	{ 0, 1, 2, 3, 7, 4, 5, 6, 8, 9,10,11}, {3,0,1,2,4,5,6,7},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0,0,0,0,0,0,0,0});

const Cube Cube::TURN_FRONT(
	{ 4, 8, 2, 3, 1, 5, 6, 7, 0, 9,10,11}, {1,5,2,3,0,4,6,7},
	{ 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0}, {1,2,0,0,2,1,0,0});
const Cube Cube::TURN_BACK(
	{ 0, 1, 6,10, 4, 5, 3, 7, 8, 9, 2,11}, {0,1,3,7,4,5,2,6},
	{ 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0}, {0,0,1,2,0,0,2,1});

const Cube Cube::TURN_MIDDLE(
	{ 0, 1, 2, 3, 8, 5, 4, 7,10, 9, 6,11}, {0,1,2,3,4,5,6,7},
	{ 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0}, {0,0,0,0,0,0,0,0});
const Cube Cube::TURN_EQUATOR(
	{ 3, 0, 1, 2, 4, 5, 6, 7, 8, 9,10,11}, {0,1,2,3,4,5,6,7},
	{ 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}, {0,0,0,0,0,0,0,0});
const Cube Cube::TURN_STANDING(
	{ 0, 1, 2, 3, 4, 9, 6, 5, 8,11,10, 7}, {0,1,2,3,4,5,6,7},
	{ 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1}, {0,0,0,0,0,0,0,0});

const Cube Cube::TURN_X = Cube::TURN_RIGHT - Cube::TURN_LEFT - Cube::TURN_MIDDLE;
const Cube Cube::TURN_Y = Cube::TURN_UP - Cube::TURN_DOWN - Cube::TURN_EQUATOR;
const Cube Cube::TURN_Z = Cube::TURN_FRONT - Cube::TURN_BACK + Cube::TURN_STANDING;

Cube::Cube() {
	memset(m_edge_orients, 0, 12 * sizeof (int));
	memset(m_corner_orients, 0, 8 * sizeof (int));
}
Cube::Cube(std::initializer_list<int> edges, std::initializer_list<int> corners,
	std::initializer_list<int> edge_orients, std::initializer_list<int> corner_orients)
	: m_edges(edges), m_corners(corners)
{
	int i = 0;
	for (auto iter = edge_orients.begin(); iter != edge_orients.end(); iter++, i++) {
		m_edge_orients[i] = *iter;
	}
	
	i = 0;
	for (auto iter = corner_orients.begin(); iter != corner_orients.end(); iter++, i++) {
		m_corner_orients[i] = *iter;
	}
}

Cube& Cube::operator=(const Cube& that) {
	m_edges = that.m_edges;
	m_corners = that.m_corners;
	for (int i = 0; i < 12; i++) {
		m_edge_orients[i] = that.m_edge_orients[i];
	}
	for (int i = 0; i < 8; i++) {
		m_corner_orients[i] = that.m_corner_orients[i];
	}
}

Cube Cube::operator+(const Cube& that) const {
	Cube result(*this);
	
	result.m_edges += that.m_edges;
	result.m_corners += that.m_corners;
	
	for (int i = 0; i < 12; i++) {
		result.m_edge_orients[i] = this->m_edge_orients[that.m_edges.element(i)] + that.m_edge_orients[i];
		result.m_edge_orients[i] %= 2;
	}
	for (int i = 0; i < 8; i++) {
		result.m_corner_orients[i] = this->m_corner_orients[that.m_corners.element(i)] + that.m_corner_orients[i];
		result.m_corner_orients[i] %= 3;
	}
	
	return result;
}
Cube& Cube::operator+=(const Cube& that) {
	operator=((*this) + that);
	return *this;
}

Cube Cube::operator-(const Cube& that) const {
	return operator+(that.inverse());
}
Cube& Cube::operator-=(const Cube& that) {
	operator+=(that.inverse());
	return *this;
}

Cube Cube::operator*(int exponent) const {
	Cube result;
	
	if (exponent > 0) {
		while(exponent > 0) {
			result += *this;
			exponent--;
		}
	} else {
		while(exponent < 0) {
			result -= *this;
			exponent++;
		}
	}
	
	return result;
}
Cube& Cube::operator*=(int exponent) {
	operator=(*this * exponent);
	return *this;
}


Cube Cube::inverse() const {
	Cube result;
	
	for (int i = 0; i < 12; i++) {
		result.m_edge_orients[this->m_edges.element(i)] = this->m_edge_orients[i];
	}
	for (int i = 0; i < 8; i++) {
		result.m_corner_orients[this->m_corners.element(i)] = (3 - this->m_corner_orients[i]) % 3;
	}
	
	result.m_corners = this->m_corners.inverse();
	result.m_edges = this->m_edges.inverse();
	
	return result;
}

Cube Cube::parse(const char* msg) {
	Cube result;
	
	char c;
	
	const Cube *last = &TURN_IDENTITY;
	
	while (c = *msg++) {
		switch (c) {
		case 'R': last = &TURN_RIGHT; break;
		case 'L': last = &TURN_LEFT; break;
		case 'U': last = &TURN_UP; break;
		case 'D': last = &TURN_DOWN; break;
		case 'F': last = &TURN_FRONT; break;
		case 'B': last = &TURN_BACK; break;
		case ' ': break;
		default: throw "unknown character";
		}
		if (msg[0] == '2') {
			result += *last * 2;
			msg++;
		} else if (msg[0] == '\'') {
			result -= *last;
			msg++;
		} else {
			result += *last;
		}
	}
	
	return result;
}