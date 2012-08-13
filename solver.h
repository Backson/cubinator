#ifndef solver_h
#define solver_h

#include "cube.h"

#include <vector>

// constants to identify all allowed turns
static const int TURN_ID_RIGHT		= 0;
static const int TURN_ID_RIGHT_I	= 1;
static const int TURN_ID_LEFT		= 2;
static const int TURN_ID_LEFT_I		= 3;
static const int TURN_ID_FRONT		= 4;
static const int TURN_ID_FRONT_I	= 5;
static const int TURN_ID_BACK		= 6;
static const int TURN_ID_BACK_I		= 7;
static const int TURN_ID_UP			= 8;
static const int TURN_ID_UP_I		= 9;
static const int TURN_ID_DOWN		= 10;
static const int TURN_ID_DOWN_I		= 11;

// convert turn id to corresponding Cube object
Cube get_cube(int i) {
	static const Cube cubes[] = {
		Cube::TURN_RIGHT, Cube::TURN_RIGHT.inverse(),
		Cube::TURN_LEFT, Cube::TURN_LEFT.inverse(),
		Cube::TURN_FRONT, Cube::TURN_FRONT.inverse(),
		Cube::TURN_BACK, Cube::TURN_BACK.inverse(),
		Cube::TURN_UP, Cube::TURN_UP.inverse(),
		Cube::TURN_DOWN, Cube::TURN_DOWN.inverse(),
	};
	
	return cubes[i];
}

// struct to hold a turn sequence produced by a solver
struct TurnSequence {
	std::vector<int> turns;
	
	TurnSequence inverse() const;
};

// abstract class for all solvers
class Solver {
public:
	virtual TurnSequence solve(const Cube&) = 0;
};

// first test solver
class MySolver : public Solver {
public:
	TurnSequence solve(const Cube&);
};

#endif // solver_h
