#include "ida_star_solver.h"

#include <cstdio>
#include <cstdlib>
#include <vector>

#include "word.h"

IdaStarSolver::IdaStarSolver(int(*heuristic)(const Cube &))
	: heuristic(heuristic), condition(nullptr)
{
	condition = [](const Cube &cube) { return cube == Cube::TURN_IDENTITY; };
}
IdaStarSolver::IdaStarSolver(int(*heuristic)(const Cube &), bool(*condition)(const Cube &))
	: heuristic(heuristic), condition(condition)
{
	// nothing
}

void IdaStarSolver::set_log(bool b) {
	log = b;
}

void IdaStarSolver::solve(const Cube& cube) {
	// depth limited search
	int search_depth = heuristic(cube);
	if (log) printf("search depth is %d\n", search_depth);

	// stack search state, so we don't have to use recursion
	stack.clear();
	stack.push_back(SearchState{cube, 0, search_depth, -1, 0, 100});

	// count how many nodes were touched (optional)
	int node_counter = 1;

	// loop until found
	while (!condition(stack.back().cube)) {
		auto &state = stack.back();
		int cost = state.cost;
		int estimate = state.estimate;
		int total = cost + estimate;

		if (stack.size() > 1 && total > search_depth) {
			// we stepped over the maximum search depth
			stack.pop_back();
			stack.back().min = std::min(stack.back().min, total);
		}
		else if (stack.back().index < (int) WORD_NUM) {
			// make another turn and add it to the stack
			int index = stack.back().index++;
			// don't check the inverse of the last turn or similar things
			if (stack.size() == 1 || !can_words_be_switched(stack.back().last_turn, index)) {
				int next_cost = cost + get_word_cost(index, metric);
				if (next_cost <= search_depth) {
					const Cube next_cube = state.cube + get_word_cube(index);
					int next_estimate = heuristic(next_cube);
					stack.push_back(SearchState{ next_cube, next_cost, next_estimate, index, 0, 100 });
					node_counter++;
				}
				else {
					state.min = std::min(state.min, next_cost);
				}
			}
		}
		else if (stack.size() == 1) {
			// increase search depth
			search_depth = state.min;
			if (log) printf("search depth now %d (nodes touched: %d)\n", search_depth, node_counter);
			state.index = 0;
			state.min = 100;
		}
		else {
			int min = state.min;
			stack.pop_back();
			stack.back().min = std::min(stack.back().min, min);
		}
	}
	if (log) printf("found after %d turns and %d touched nodes.\n", stack.back().cost, node_counter);
}

void IdaStarSolver::print_solution() {
	int num = get_solution_turn_number();
	printf("%d turns: ", num);
	for (int i = 1; i < (int)stack.size(); ++i) {
		const char *str[] = {
			"R", "R2", "R'", "L", "L R", "L R2", "L R'", "L2", "L2 R", "L2 R2", "L2 R'",
			"L'", "L' R", "L' R2", "L' R'",
			"B", "B2", "B'", "F", "F B", "F B2", "F B'", "F2", "F2 B", "F2 B2", "F2 B'",
			"F'", "F' B", "F' B2", "F' B'",
			"D", "D2", "D'", "U", "U D", "U D2", "U D'", "U2", "U2 D", "U2 D2", "U2 D'",
			"U'", "U' D", "U' D2", "U' D'",
		};
		printf("(%s) ", str[stack[i].last_turn]);
	}
}

Cube IdaStarSolver::get_solution() {
	Cube cube = Cube::TURN_IDENTITY;
	for (int i = 1; i < (int)stack.size(); ++i) {
		cube += get_word_cube(stack[i].last_turn);
	}
	return cube;
}

int IdaStarSolver::get_solution_turn_number() {
	return stack.back().cost;
}
