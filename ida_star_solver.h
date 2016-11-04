#ifndef ida_star_solver_h
#define ida_star_solver_h

#include "cube.h"

class IdaStarSolver {
public:
	IdaStarSolver(int(*heuristic)(const Cube &)) : heuristic(heuristic) {}
	void solve(const Cube &);
private:
	int(*heuristic)(const Cube &);
};

#endif // ida_star_solver_h
