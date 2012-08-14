#ifndef ida_star_h
#define ida_star_h

#include "solver.h"

// first test solver
class IdaStarSolver : public Solver {
public:
	TurnSequence solve(const Cube&);
};

#endif // ida_star_h
