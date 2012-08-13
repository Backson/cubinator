#include "cube.h"

#include <string.h>

const Cube Cube::TURN_IDENTITY;

const Cube Cube::TURN_RIGHT(TURN_RIGHT_INITIALIZER);
const Cube Cube::TURN_LEFT(TURN_LEFT_INITIALIZER);

const Cube Cube::TURN_UP(TURN_UP_INITIALIZER);
const Cube Cube::TURN_DOWN(TURN_DOWN_INITIALIZER);

const Cube Cube::TURN_FRONT(TURN_FRONT_INITIALIZER);
const Cube Cube::TURN_BACK(TURN_BACK_INITIALIZER);

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

bool Cube::operator==(const Cube& that) {
	if (this->m_edges != that.m_edges) return false;
	if (this->m_corners != that.m_corners) return false;
	for (int i = 0; i < 12; i++) {
		if (this->m_edge_orients[i] != that.m_edge_orients[i]) return false;
	}
	for (int i = 0; i < 8; i++) {
		if (this->m_corner_orients[i] != that.m_corner_orients[i]) return false;
	}
	return true;
}

bool Cube::operator!=(const Cube& that) {
	return !operator==(that);
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
		case ' ': continue;
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