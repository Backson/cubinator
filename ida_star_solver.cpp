#include "ida_star_solver.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

static int counter = -1;

#define BIG_NUMBER (INT_MAX/2)

int heuristic_cost(const Cube& cube) {
	const Cube& ident = Cube::TURN_IDENTITY;
	int result = 0;
	
	int edge_coordinates[12][3] = {
		{0,0,1},{2,0,1},{2,2,1},{0,2,1},
		{1,0,0},{2,1,0},{1,2,0},{0,1,0},
		{1,0,2},{2,1,2},{1,2,2},{0,1,2}
	};
	
	int corner_coordinates[8][3] = {
		{0,0,0},{2,0,0},{2,2,0},{0,2,0},
		{0,0,2},{2,0,2},{2,2,2},{0,2,2},
	};
	int edges = 0;
	for (int i = 0; i < 12; i++) {
		auto coord1 = edge_coordinates[cube.edges().element(i)];
		auto coord2 = edge_coordinates[ident.edges().element(i)];
		edges += abs(coord1[0]-coord2[0])+abs(coord1[1]-coord2[1])+abs(coord1[2]-coord2[2]);
		/*if (cube.edges().element(i) == ident.edges().element(i)) {
			if (cube.edge_orients()[i] == ident.edge_orients()[i]) {
				edges += 200;
			}
		}*/
	}
	int corners = 0;
	for (int i = 0; i < 8; i++) {
		auto coord1 = corner_coordinates[cube.corners().element(i)];
		auto coord2 = corner_coordinates[ident.corners().element(i)];
		corners += abs(coord1[0]-coord2[0])+abs(coord1[1]-coord2[1])+abs(coord1[2]-coord2[2]);
		/*if (cube.corners().element(i) == ident.corners().element(i)) {

			if (cube.corner_orients()[i] == ident.corner_orients()[i]) {
				//result += 200;
			}
		}*/
	}

	result = edges > corners ? edges : corners;
	return result / 8;
}

struct solution_s {
	TurnSequence solution;
	int cost_limit;
};

solution_s depth_limited_search(const Cube& cube, solution_s sol, int start_cost) {
	//printf("depth %ld: with start cost %d\n", sol.solution.turns.size(), start_cost);
	counter++;
	int old_estimated_cost = heuristic_cost(cube);
	int minimum_cost = start_cost + heuristic_cost(cube);
    if (minimum_cost > sol.cost_limit)
        return {{}, minimum_cost};
    if (cube == Cube::TURN_IDENTITY) {
        //printf("got it!\n");
		return sol;
	}
	
	int last_turn = -1;
	if (sol.solution.turns.size() > 0) {
		last_turn = *sol.solution.turns.rbegin();
	}
	
    int next_cost_limit = BIG_NUMBER;
    for (int i = 0; i < 12; i++) {
		Cube turn = get_cube(i);
		Cube successor = cube + turn;
		
		int new_start_cost;
		if (last_turn != -1 && i == last_turn - (last_turn % 2) * 2 + 1)
			new_start_cost = BIG_NUMBER;
		else
			new_start_cost = start_cost - old_estimated_cost + heuristic_cost(successor) + 1;
		
		solution_s new_path(sol);
		new_path.solution.turns.push_back(i);
        solution_s result = depth_limited_search(successor, {new_path.solution, sol.cost_limit}, new_start_cost);
		
		TurnSequence solution = result.solution;
		int new_cost_limit = result.cost_limit;
		
        if (solution.turns.size() > 0)
            return {solution, new_cost_limit};
        next_cost_limit = next_cost_limit < new_cost_limit ? next_cost_limit : new_cost_limit;
	}
 
    return {{}, next_cost_limit};
}

TurnSequence IdaStarSolver::solve(const Cube& cube) {
    int cost_limit = heuristic_cost(cube);
	TurnSequence solution;
	
	int iterations = 0;
	
    while (true) {
		
		printf("Iteration %d:", ++iterations);
		fflush(stdout);
		counter = 0;
		
        auto result = depth_limited_search(cube, {solution, cost_limit}, 0);
		solution = result.solution;
		cost_limit = result.cost_limit;
		printf(" %d nodes visited.\n", counter);
		
        if (solution.turns.size() > 0)
            return solution;
        if (cost_limit >= BIG_NUMBER) {
			//printf("oh noes!\n");
            return {};
		}
	}
}
