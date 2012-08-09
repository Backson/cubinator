#ifndef cube_h
#define cube_h

#include "permut.h"

#include <initializer_list>

class Cube {
public:
	
	static const Cube TURN_IDENTITY;
	
	static const Cube TURN_RIGHT;
	static const Cube TURN_LEFT;
	
	static const Cube TURN_UP;
	static const Cube TURN_DOWN;
	
	static const Cube TURN_FRONT;
	static const Cube TURN_BACK;

	Cube();
	Cube(const Cube& that) { this->operator=(that); }
	Cube(std::initializer_list<int> edges, std::initializer_list<int> corners,
		std::initializer_list<int> edge_orients, std::initializer_list<int> corner_orients);
	~Cube() = default;
	
	Cube& operator=(const Cube&);
	
	Cube operator+(const Cube&) const;
	Cube& operator+=(const Cube&);
	
	Cube operator-(const Cube&) const;
	Cube& operator-=(const Cube&);
	
	Cube operator*(int exponent) const;
	Cube& operator*=(int exponent);
	
	const Perm<12>& edges() const { return m_edges; }
	const Perm<8>& corners() const { return m_corners; }
	
	const int* edge_orients() const { return m_edge_orients; }
	const int* corner_orients() const { return m_corner_orients; }
	
	Cube inverse() const;
	
	static Cube parse(const char*);

private:

Perm<12> m_edges;
Perm<8> m_corners;

int m_edge_orients[12];
int m_corner_orients[8];

};

#endif // cube_h
