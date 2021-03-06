#ifndef ida_star_solver_h
#define ida_star_solver_h

#include "cube.h"
#include "word.h"

#include <vector>

class IdaStarSolver {
public:
	IdaStarSolver(int(*heuristic)(const Cube &));
	IdaStarSolver(int(*heuristic)(const Cube &), bool(*condition)(const Cube &));
	void solve(const Cube &);
	void print_solution();
	void set_log(bool b = true);
	Cube get_solution();
	int get_solution_turn_number();
private:
	// used to track the state of the search algorithm
	struct SearchState {
		// puzzle position at the current node
		Cube cube;
		// cost to get from the root to this node
		int cost;
		// estimated cost to get from this node to the target
		int estimate;
		// index of last turn from previous position
		int last_turn;
		// how many successors of this have already been tested
		int index;
		// minimum cost of all successors
		int min;
	};

	std::vector<SearchState> stack;
	int(*heuristic)(const Cube &);
	bool(*condition)(const Cube &);
	bool log = false;
	int metric = METRIC_DEFAULT;
};

#endif // ida_star_solver_h
