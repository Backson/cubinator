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

int dumb_heuristic(const Cube &cube) {
	return cube == Cube::TURN_IDENTITY ? 0 : 1;
}

static int *table = nullptr;
static unsigned int table_size = 0;

unsigned int hash_function(const Cube &cube) {
	unsigned int hash = 0xdf9af575;

	for (int i = 0; i < 8; ++i){
		hash = cube.corners().element(i) + (hash << 6) + (hash << 16) - hash;
		hash = cube.corner_orients()[i] + (hash << 6) + (hash << 16) - hash;
		hash = (hash << 1) | (hash >> 31);
	}
	for (int i = 0; i < 12; ++i){
		hash = cube.edges().element(i) + (hash << 6) + (hash << 16) - hash;
		hash = cube.edge_orients()[i] + (hash << 6) + (hash << 16) - hash;
		hash = (hash << 1) | (hash >> 31);
	}
	//hash = hash << 4 | (cube.corners().inversions() & 0x3) << 2 | (cube.edge_orients()[0] & 0x1) << 1 | cube.edge_orients()[2] & 0x1;
	return hash;
};

int smart_heuristic(const Cube &cube) {
	unsigned int hash = hash_function(cube);
	int result = table[hash % table_size];
	return result;
}

void compute_heuristic(int n, int table_bits) {
	table_size = 1 << table_bits;

	if (table != nullptr) delete[] table;
	table = new int[table_size];

	printf("n = %2d, s = 0x%05x...\n", n, table_size);
	
	for (int i = 0; i < table_size; ++i)
		table[i] = n + 1;

	std::function<void (int, Cube)> visitor = [&](int m, Cube cube) {
		int ind = hash_function(cube) % table_size;
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

	int histo[30];
	for (int i = 0; i < 30; ++i)
		histo[i] = 0;

	std::default_random_engine rng;
	rng.seed(53);
	for (int i = 0; i < 1000000; ++i) {
		Cube cube = get_random_cube(rng);
		int ind = hash_function(cube) % table_size;
		int table_entry = table[ind];
		histo[table_entry]++;
	}

	int histo_heuristic_costs[30];
	for (int i = 0; i < 30; ++i)
		histo_heuristic_costs[i] = 0;

	// dump the table into a file
	char filename[100];
	sprintf(filename, "table_%d_%d.txt", n, table_bits);
	FILE *file = fopen(filename, "w");
	for (int i = 0; i < table_size; ++i) {
		histo_heuristic_costs[table[i]]++;
		fprintf(file, "%d, ", table[i]);
		if ((i + 1) % 20 == 0)
			fprintf(file, "\n");
	}
	fclose(file);
	
	// print histogram of random scrambles
	for (int i = 0; i <= n + 1; ++i)
		printf("%2d: %6d  %6d\n", i, histo[i], histo_heuristic_costs[i]);
	printf("\n");
}

int main(int argc, char *argv[]) {
	std::default_random_engine rng;
	rng.seed(42);

	compute_heuristic(7, 16);

	Cube cube1 = get_random_cube(rng, 7);
	Cube cube2 = get_random_cube(rng, 10);
	
	IdaStarSolver dumb_solver(&dumb_heuristic);
	IdaStarSolver smart_solver(&smart_heuristic);

	printf("dumb solver:\n");
	dumb_solver.solve(cube1);
	printf("smart solver:\n");
	smart_solver.solve(cube1);
	printf("smart solver, harder cube:\n");
	smart_solver.solve(cube2);

	if (table != nullptr) delete[] table;
	getchar();
	return 0;
}
