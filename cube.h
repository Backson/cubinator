#ifndef cube_h
#define cube_h

#define C_FL 0
#define C_FR 1
#define C_BR 2
#define C_BL 3
#define C_DF 4
#define C_DR 5
#define C_DB 6
#define C_DL 7
#define C_UF 8
#define C_UR 9
#define C_UB 10
#define C_UL 11

#define C_DFL 0
#define C_DRF 1
#define C_DBR 2
#define C_DLB 3
#define C_ULF 4
#define C_UFR 5
#define C_URB 6
#define C_UBL 7

#define TURN_RIGHT_INITIALIZER \
	{ 0, 5, 9, 3, 4, 2, 6, 7, 8, 1,10,11}, {0,2,6,3,4,1,5,7},\
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0,1,2,0,0,2,1,0}
#define TURN_LEFT_INITIALIZER \
	{11, 1, 2, 7, 4, 5, 6, 0, 8, 9,10, 3}, {4,1,2,0,7,5,6,3},\
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {2,0,0,1,1,0,0,2}
#define TURN_UP_INITIALIZER \
	{ 0, 1, 2, 3, 4, 5, 6, 7, 9,10,11, 8}, {0,1,2,3,5,6,7,4},\
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0,0,0,0,0,0,0,0}
#define TURN_DOWN_INITIALIZER \
	{ 0, 1, 2, 3, 7, 4, 5, 6, 8, 9,10,11}, {3,0,1,2,4,5,6,7},\
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0,0,0,0,0,0,0,0}
#define TURN_FRONT_INITIALIZER \
	{ 4, 8, 2, 3, 1, 5, 6, 7, 0, 9,10,11}, {1,5,2,3,0,4,6,7},\
	{ 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0}, {1,2,0,0,2,1,0,0}
#define TURN_BACK_INITIALIZER \
	{ 0, 1, 6,10, 4, 5, 3, 7, 8, 9, 2,11}, {0,1,3,7,4,5,2,6},\
	{ 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0}, {0,0,1,2,0,0,2,1}

#include "permut.h"

#include <initializer_list>
#include <array>

class Cube {
public:
	
	static const Cube TURN_IDENTITY;
	
	static const Cube TURN_RIGHT;
	static const Cube TURN_LEFT;
	
	static const Cube TURN_UP;
	static const Cube TURN_DOWN;
	
	static const Cube TURN_FRONT;
	static const Cube TURN_BACK;

	class Metric {
	public:
		static int size() { return 12; }
		static const Cube &get(int i) {
			static const std::array<Cube, 12> all_turns = {
				Cube::TURN_RIGHT, Cube::TURN_RIGHT.inverse(),
				Cube::TURN_LEFT, Cube::TURN_LEFT.inverse(),
				Cube::TURN_FRONT, Cube::TURN_FRONT.inverse(),
				Cube::TURN_BACK, Cube::TURN_BACK.inverse(),
				Cube::TURN_UP, Cube::TURN_UP.inverse(),
				Cube::TURN_DOWN, Cube::TURN_DOWN.inverse(),
			};
			return all_turns[i];
		}
	};
	
	Cube();
	Cube(const Cube& that) { this->operator=(that); }
	Cube(std::initializer_list<int> edges, std::initializer_list<int> corners,
		std::initializer_list<int> edge_orients, std::initializer_list<int> corner_orients);
	Cube(Perm<12> edges, Perm<8> corners, const int edge_orients[12], const int corner_orients[8]);
	~Cube() = default;
	
	Cube& operator=(const Cube&);
	
	Cube operator+(const Cube&) const;
	Cube& operator+=(const Cube&);
	
	Cube operator-(const Cube&) const;
	Cube& operator-=(const Cube&);
	
	Cube operator*(int exponent) const;
	Cube& operator*=(int exponent);
	
	bool operator==(const Cube&) const;
	bool operator!=(const Cube&) const;
	
	const Perm<12>& edges() const { return m_edges; }
	const Perm<8>& corners() const { return m_corners; }
	
	const int* edge_orients() const { return m_edge_orients; }
	const int* corner_orients() const { return m_corner_orients; }
	
	int correct_edges() const;
	int correct_corners() const;

	bool solvable() const;
	
	Cube inverse() const;
	
	static Cube parse(const char*);

private:

	Perm<12> m_edges;
	Perm<8> m_corners;

	int m_edge_orients[12];
	int m_corner_orients[8];

};

#endif // cube_h
