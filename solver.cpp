#include "solver.h"

#include <stack>

TurnSequence TurnSequence::inverse() const {
	
	auto length = this->turns.size();
	
	TurnSequence result;
	result.turns.reserve(length);
	
	for (int i = 0; i < length; i++) {
		result.turns[i] += this->turns[i] % 2 ? +1 : -1;
	}
}

// internal class for the solver algorithm
struct PositionTree {
	Cube cube;
	PositionTree *children[12];
	int values[12];
	
	PositionTree();
	PositionTree(const Cube&);
	~PositionTree();
};
PositionTree::PositionTree() {
	memset(children, 0, 12 * sizeof (PositionTree*) + 12 * sizeof (int));
}
PositionTree::PositionTree(const Cube& cube) {
	this->cube = cube;
	memset(children, 0, 12 * sizeof (PositionTree*) + 12 * sizeof (int));
}
PositionTree::~PositionTree() {
	for (int i = 0; i < 12; i++) {
		if (children[i])
			delete children[i];
	}
}

TurnSequence MySolver::solve(const Cube& cube) {
	// TODO
}