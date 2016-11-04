#include <cstdio>
#include <cstdlib>
#include <random>
#include <functional>

#include "cube.h"
#include "cube_ex.h"
#include "cube_random.h"
#include "ida_star_solver.h"

template<int length>
void print_perm(Perm<length> perm) {
	for (int i = 0; i < length; i++)
		printf("%d ", perm.element(i));
	printf("\n");
}

/// visual ASCII representation of the top side of the cube
void draw_up(const ExtendedCube& cube) {
	static const char corner_stones[8][3] = {
		{'w','o','b'},
		{'w','b','r'},
		{'w','r','g'},
		{'w','g','o'},
		{'y','b','o'},
		{'y','r','b'},
		{'y','g','r'},
		{'y','o','g'}};
	static const char edge_stones[12][2] = {
		{'b','o'},
		{'b','r'},
		{'g','r'},
		{'g','o'},
		{'w','b'},
		{'w','r'},
		{'w','g'},
		{'w','o'},
		{'y','b'},
		{'y','r'},
		{'y','g'},
		{'y','o'}};
	static const char middle_stones[6] = {'y','b','r','g','o','w'};
	
	char stone[9];

	stone[0] = corner_stones[cube.corners().element(7)][(3-cube.corner_orients()[7]) % 3];
	stone[1] = edge_stones[cube.edges().element(10)][cube.edge_orients()[10]];
	stone[2] = corner_stones[cube.corners().element(6)][(3-cube.corner_orients()[6]) % 3];

	stone[3] = edge_stones[cube.edges().element(11)][cube.edge_orients()[11]];
	stone[4] = middle_stones[cube.middles().element(0)];
	stone[5] = edge_stones[cube.edges().element(9)][cube.edge_orients()[9]];

	stone[6] = corner_stones[cube.corners().element(4)][(3-cube.corner_orients()[4]) % 3];
	stone[7] = edge_stones[cube.edges().element(8)][cube.edge_orients()[8]];
	stone[8] = corner_stones[cube.corners().element(5)][(3-cube.corner_orients()[5]) % 3];

	printf(" %c %c %c \n", stone[0], stone[1], stone[2]);
	printf(" %c %c %c \n", stone[3], stone[4], stone[5]);
	printf(" %c %c %c \n", stone[6], stone[7], stone[8]);
}

/// visual ASCII representation of the whole cube
void draw_cube(const ExtendedCube& cube) {
	const char* side_names[6] = {
	"UP", "DOWN", "FRONT", "RIGHT", "BACK", "LEFT"};
	
	const ExtendedCube turns[6] = {
		ExtendedCube::TURN_IDENTITY,
		ExtendedCube::TURN_X*2,
		ExtendedCube::TURN_X,
		ExtendedCube::TURN_Z*(-1) + ExtendedCube::TURN_Y,
		ExtendedCube::TURN_X*(-1) + ExtendedCube::TURN_Y * 2,
		ExtendedCube::TURN_Z + ExtendedCube::TURN_Y*(-1)
	};
	
	for(int i = 0; i < 6; i++) {
		printf("%s\n", side_names[i]);
		draw_up(cube + turns[i]);
		if(i < 5) {
			printf("+-----+\n");
		}
	}
}

void compute_heuristic(int n) {
	int table_size = 0x8000;
	int *table = new int[table_size];
	for (int i = 0; i < table_size; ++i)
		table[i] = n;

	std::function<int(const Cube &)> index = [](const Cube &cube) {
		unsigned int hash = 0xdf9af575;
		for (int i = 0; i < 8; ++i){
			hash = cube.corners().element(i) + (hash << 6) + (hash << 16) - hash;
			hash = cube.corner_orients()[i] + (hash << 6) + (hash << 16) - hash;
		}
		for (int i = 0; i < 12; ++i){
			hash = cube.edges().element(i) + (hash << 6) + (hash << 16) - hash;
			hash = cube.edge_orients()[i] + (hash << 6) + (hash << 16) - hash;
		}
		return (hash % 0x8000);
	};

	std::function<void (int, Cube)> visitor = [&](int m, Cube cube) {
		int ind = index(cube);
		table[ind] = std::min(table[ind], m);
	};
	std::function<void (int, int, Cube, int)> search = [&](int n, int m, Cube cube, int last_turn) {
		visitor(m, cube);
		if (n == m) return;
		for (int i = 0; i < Cube::Metric::size(); ++i) {
			if (i != (last_turn ^ 1))
				search(n, m + 1, cube + Cube::Metric::get(i), i);
		}
	};

	search(n, 0, Cube::TURN_IDENTITY, -1);

	//for (int i = 0; i < 9; ++i) printf("%d: %d\n", i, table[i]);

	int histo[20];
	for (int i = 0; i < 20; ++i)
		histo[i] = 0;

	std::default_random_engine rng;
	rng.seed(23);
	for (int i = 0; i < 100000; ++i) {
		Cube cube = get_random_cube(rng);
		int ind = index(cube);
		int table_entry = table[ind];
		histo[table_entry]++;
	}
	
	printf("\n");
	for (int i = 0; i <= n; ++i)
		printf("%d: %d\n", i, histo[i]);

	delete[] table;
}

int main(int argc, char *argv[]) {
	compute_heuristic(6);
	
	getchar();
	return 0;

	std::default_random_engine rng;
	rng.seed(42);

	Cube cube = get_random_cube(rng);
	draw_cube(cube);

	IdaStarSolver solver;
	printf("\nSolution:\n");
	solver.solve(cube);

	getchar();
	return 0;
}
