#include "ida_star_solver.h"

#include <cstdio>
#include <cstdlib>
#include <vector>

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

void IdaStarSolver::solve(const Cube& cube) {
	// depth limited search
	int search_depth = heuristic(cube);

	// stack search state, so we don't have to use recursion
	std::vector<SearchState> stack;
	stack.push_back(SearchState{cube, 0, search_depth, -1, 0, 100});

	// count how many nodes were touched (optional)
	int node_counter = 1;

	// loop until found
	while (stack.back().cube != Cube::TURN_IDENTITY) {
		auto &state = stack.back();
		int cost = state.cost;
		int estimate = state.estimate;
		int total = cost + estimate;
		if (total > search_depth && cost != 0) {
			// if we stepped over the maximum search depth:
			stack.pop_back();
			stack.back().min = std::min(stack.back().min, total);
			// optional: correct the cost estimate of the previous node
			stack.back().estimate = std::max(stack.back().estimate, estimate - 1);
		} else if (stack.back().index < (int) Cube::Metric::size()) {
			// make another turn and add it to the stack
			int index = stack.back().index++;
			// don't check the inverse of the last turn
			if ((stack.back().last_turn ^ index) == 1) {
				continue;
			}
			// don't check the same turn 3 times in a row
			if (stack.size() >= 2) {
				int last_turn1 = stack[stack.size() - 1].last_turn;
				int last_turn2 = stack[stack.size() - 2].last_turn;
				if (last_turn1 == index && last_turn2 == index) {
					continue;
				}
			}
			const Cube next_cube = state.cube + Cube::Metric::get(index);
			int next_cost = state.cost + 1;
			stack.push_back(SearchState{next_cube, next_cost, heuristic(next_cube), index, 0, 100});
			node_counter++;
		} else if (state.cost == 0) {
			// increase search depth
			search_depth = state.min;
			printf("search depth now %d (nodes touched: %d)\n", search_depth, node_counter);
			state.index = 0;
			state.min = 100;
		} else {
			int min = state.min;
			stack.pop_back();
			stack.back().min = std::min(stack.back().min, min);
		}
	}

	// print result
	for (int i = 1; i < (int) stack.size(); ++i) {
		const char *str[] = { "R", "R'", "L", "L'", "F", "F'", "B", "B'", "U", "U'", "D", "D'" };
		printf("%s ", str[stack[i].last_turn]);
	}
	printf("\n\n");
}
