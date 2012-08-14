#include "solver.h"

#include <stdio.h>
#include <set>

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

TurnSequence inverse(const TurnSequence& that) {
	
	auto length = that.turns.size();
	
	TurnSequence result;
	result.turns.reserve(length);
	
	for (int i = 0; i < length; i++) {
		result.turns[i] += that.turns[i] % 2 ? +1 : -1;
	}
}

// internal class for the solver algorithm
struct PositionTree {
	PositionTree *parent;
	Cube cube;
	int turn;
	int value;
	PositionTree *children[12];
	
	PositionTree() = delete;
	PositionTree(const Cube&);
	PositionTree(PositionTree* parent, int turn);
	~PositionTree();
};

inline int abs(int i) {
	return i < 0 ? -i : i;
}

int get_value(const PositionTree* tree) {
	const Cube& cube = tree->cube;
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
		edges -= abs(coord1[0]-coord2[0])+abs(coord1[1]-coord2[1])+abs(coord1[2]-coord2[2]);
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
		corners -= abs(coord1[0]-coord2[0])+abs(coord1[1]-coord2[1])+abs(coord1[2]-coord2[2]);
		/*if (cube.corners().element(i) == ident.corners().element(i)) {

			if (cube.corner_orients()[i] == ident.corner_orients()[i]) {
				//result += 200;
			}
		}*/
	}
	corners /= 2;
	edges /= 2;
	result = edges < corners ? edges : corners;
	return corners;
}

int get_value_broadsearch(const PositionTree* tree) {
	int result = tree->parent ? tree->parent->value - 1 : 0;
}

PositionTree::PositionTree(const Cube& cube) {
	this->parent = NULL;
	this->cube = cube;
	this->turn = -1;
	this->value = get_value(this);
	memset(children, 0, 12 * sizeof (PositionTree*));
}
PositionTree::PositionTree(PositionTree* parent, int turn) {
	this->parent = parent;
	this->cube = parent->cube + get_cube(turn);
	this->turn = turn;
	this->value = get_value(this);
	memset(children, 0, 12 * sizeof (PositionTree*));
}
PositionTree::~PositionTree() {
	for (int i = 0; i < 12; i++) {
		if (children[i])
			delete children[i];
	}
}

class MyComparator {
private:

public:
    bool operator()(const PositionTree* c1, const PositionTree* c2)
    {
        return c1->value < c2->value;
    }
};

TurnSequence MySolver::solve(const Cube& cube) {
	PositionTree* root = new PositionTree(cube);
	
	if (root->cube == Cube::TURN_IDENTITY)
		return TurnSequence();
	
	std::set<PositionTree*> positions;
	positions.insert(root);
	
	// search for the solution
	PositionTree* current = NULL;
	while (true) {
		current = *(positions.begin());
		
		printf("before: %ld: %+4d to %+4d\n", positions.size(), (*positions.rbegin())->value, current->value);
		
		/*for (auto iter = positions.rbegin(); iter != positions.rend(); iter++) {
			if ((*iter)->value < current->value - 1) {
				positions.erase(--iter.base());
			} else {
				break;
			}
		}
		
		printf("after:  %ld: %+4d to %+4d\n", positions.size(), (*positions.rbegin())->value, current->value);
		*/
		positions.erase(current);
		
		PositionTree* solution = NULL;
		for (int i = 0; i < 12; i++) {
			if (i == (current->turn % 2 ? current->turn + 1 : current->turn - 1))
				continue;
			PositionTree *branch = new PositionTree(current, i);
			current->children[i] = branch;
			positions.insert(branch);
			if (branch->cube.corners() == Cube::TURN_IDENTITY.corners())
				solution = branch;
		}
		if (solution) {
			current = solution;
			break;
		}
	}
	
	PositionTree* solution = current;
	
	// determine length of the sequence
	int length = 0;
	while (current->parent != NULL) {
		current = current->parent;
		length++;
	}
	// extract turn sequence from the stack
	TurnSequence result;
	if (length <= 0) return result;
	result.turns.resize(length);
	while (length > 0) {
		result.turns[length - 1] = solution->turn;
		solution = solution->parent;
		length--;
	}
	delete root;
	printf("\n");
	return result;
}