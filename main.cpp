#include <cstdio>
#include <cstdlib>
#include <random>
#include <functional>
#include <chrono>
#include <iostream>
#include <fstream>

#include <omp.h>

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

static int *g_table1 = nullptr;
static int *g_table2 = nullptr;
static int *g_table3 = nullptr;
static unsigned int g_table_size = 0;

unsigned int hash_function_corners(const Cube &cube) {
	unsigned int hash = 0xf2f57582;

	for (int i = 0; i < 8; ++i){
		hash = cube.corners().element(i) + (hash << 6) + (hash << 16) - hash;
		hash = cube.corner_orients()[i] + (hash << 6) + (hash << 16) - hash;
		hash = (hash << 1) | (hash >> 31);
	}
	return hash;
};

unsigned int hash_function_upper_edges(const Cube &cube) {
	unsigned int hash = 0xae7cfd5f;

	for (int i = 0; i < 6; ++i){
		hash = cube.edges().element(i) + (hash << 6) + (hash << 16) - hash;
		hash = cube.edge_orients()[i] + (hash << 6) + (hash << 16) - hash;
		hash = (hash << 1) | (hash >> 31);
	}
	return hash;
};

unsigned int hash_function_lower_edges(const Cube &cube) {
	unsigned int hash = 0xb474a585;

	for (int i = 6; i < 12; ++i){
		hash = cube.edges().element(i) + (hash << 6) + (hash << 16) - hash;
		hash = cube.edge_orients()[i] + (hash << 6) + (hash << 16) - hash;
		hash = (hash << 1) | (hash >> 31);
	}
	return hash;
};

int smart_heuristic1(const Cube &cube) {
	unsigned int hash = hash_function_corners(cube);
	int result = g_table1[hash % g_table_size];
	return result;
}

int smart_heuristic2(const Cube &cube) {
	unsigned int hash = hash_function_upper_edges(cube);
	int result = g_table2[hash % g_table_size];
	return result;
}

int smart_heuristic3(const Cube &cube) {
	unsigned int hash = hash_function_lower_edges(cube);
	int result = g_table3[hash % g_table_size];
	return result;
}

int smart_heuristic(const Cube &cube) {
	int result1 = smart_heuristic1(cube);
	int result2 = smart_heuristic2(cube);
	int result3 = smart_heuristic3(cube);
	return std::max(std::max(result1, result2), result3);
}

static void search_visitor(int m, Cube cube, int *table, int table_size) {
	int ind = hash_function_corners(cube) % table_size;
	table[table_size * 0 + ind] = std::min(table[table_size * 0 + ind], m);
	ind = hash_function_upper_edges(cube) % table_size;
	table[table_size * 1 + ind] = std::min(table[table_size * 1 + ind], m);
	ind = hash_function_lower_edges(cube) % table_size;
	table[table_size * 2 + ind] = std::min(table[table_size * 2 + ind], m);
};

static void search_traversal(int n, int m, Cube cube, int last_turn, int *table, int table_size) {
	search_visitor(m, cube, table, table_size);
	if (n == m) return;
	for (int i = 0; i < Cube::Metric::size(); ++i) {
		if (i != (last_turn ^ 1))
			search_traversal(n, m + 1, cube + Cube::Metric::get(i), i, table, table_size);
	}
};

void compute_heuristic_single(int n, const Cube &start, int bias) {
	auto table_size = g_table_size;
	int *local_table = new int[table_size * 3];

	for (unsigned int i = 0; i < table_size * 3; ++i)
		local_table[i] = n + 1;

	search_traversal(n, 0, start, -1, local_table, table_size);

	#pragma omp critical
	for (unsigned int i = 0; i < table_size; ++i) {
		int old = g_table1[i];
		int current = local_table[table_size * 0 + i] + bias;
		g_table1[i] = current < old ? current : old;
		old = g_table2[i];
		current = local_table[table_size * 1 + i] + bias;
		g_table2[i] = current < old ? current : old;
		old = g_table3[i];
		current = local_table[table_size * 2 + i] + bias;
		g_table3[i] = current < old ? current : old;
	}

	delete[] local_table;
}

void compute_heuristic(int n, int table_bits, bool parallel = false) {
	g_table_size = 1 << table_bits;

	if (g_table1 != nullptr) delete[] g_table1;
	g_table1 = new int[g_table_size];
	if (g_table2 != nullptr) delete[] g_table2;
	g_table2 = new int[g_table_size];
	if (g_table3 != nullptr) delete[] g_table3;
	g_table3 = new int[g_table_size];

	printf("n = %2d, s = 0x%05x...\n", n, g_table_size);
	
	for (unsigned int i = 0; i < g_table_size; ++i) {
		g_table1[i] = n + 1;
		g_table2[i] = n + 1;
		g_table3[i] = n + 1;
	}

	if (parallel) {
		#pragma omp parallel for shared(g_table1, g_table2, g_table3, g_table_size)
		for (int i = 0; i < Cube::Metric::size(); ++i) {
			int tid = omp_get_thread_num();
			int tnum = omp_get_num_threads();
			printf("Hi, thread %d of %d clocking in!\n", tid, tnum);
			const Cube &start = Cube::Metric::get(i);
			compute_heuristic_single(n, start, 1);
		}
	} else {
		for (int i = 0; i < Cube::Metric::size(); ++i) {
			int tid = omp_get_thread_num();
			int tnum = omp_get_num_threads();
			printf("Hi, thread %d of %d clocking in!\n", tid, tnum);
			const Cube &start = Cube::Metric::get(i);
			compute_heuristic_single(n, start, 1);
		}
	}
	
	// dump the table into a file
	char filename[100];
	sprintf(filename, "table_%d_%d_1%s.txt", n, table_bits, (parallel ? "_p" : ""));
	FILE *file1 = fopen(filename, "w");
	sprintf(filename, "table_%d_%d_2%s.txt", n, table_bits, (parallel ? "_p" : ""));
	FILE *file2 = fopen(filename, "w");
	sprintf(filename, "table_%d_%d_3%s.txt", n, table_bits, (parallel ? "_p" : ""));
	FILE *file3 = fopen(filename, "w");
	for (unsigned int i = 0; i < g_table_size; ++i) {
		fprintf(file1, "%d, ", g_table1[i]);
		fprintf(file2, "%d, ", g_table2[i]);
		fprintf(file3, "%d, ", g_table3[i]);
		if ((i + 1) % 20 == 0) {
			fprintf(file1, "\n");
			fprintf(file2, "\n");
			fprintf(file3, "\n");
		}
	}
	fclose(file1);
	fclose(file2);
	fclose(file3);
	

}

void heuristic_statistic() {
	int histo[30];
	for (int i = 0; i < 30; ++i)
		histo[i] = 0;

	std::default_random_engine rng;
	rng.seed(53);
	for (int i = 0; i < 1000000; ++i) {
		Cube cube = get_random_cube(rng);
		int table_entry = smart_heuristic(cube);
		histo[table_entry]++;
	}

	// print histogram of random scrambles
	for (int i = 0; i <= 20; ++i)
		printf("%2d: %7d\n", i, histo[i]);
	printf("\n");
}

void load_heuristic(int n, int table_bits) {
	g_table_size = 1 << table_bits;
	bool parallel = true;
	
	if (g_table1 != nullptr) delete[] g_table1;
	g_table1 = new int[g_table_size];
	if (g_table2 != nullptr) delete[] g_table2;
	g_table2 = new int[g_table_size];
	if (g_table3 != nullptr) delete[] g_table3;
	g_table3 = new int[g_table_size];

	char filename[100];
	
	std::fstream ifs1;
	sprintf(filename, "table_%d_%d_1%s.txt", n, table_bits, (parallel ? "_p" : ""));
	ifs1.open(filename, std::ios_base::in);
	std::fstream ifs2;
	sprintf(filename, "table_%d_%d_2%s.txt", n, table_bits, (parallel ? "_p" : ""));
	ifs2.open(filename, std::ios_base::in);
	std::fstream ifs3;
	sprintf(filename, "table_%d_%d_3%s.txt", n, table_bits, (parallel ? "_p" : ""));
	ifs3.open(filename, std::ios_base::in);

	for (int i = 0; i < g_table_size; ++i) {
		int val;
		char c;

		ifs1 >> val;
		ifs1.get(c);
		g_table1[i] = val;

		ifs2 >> val;
		ifs2.get(c);
		g_table2[i] = val;

		ifs3 >> val;
		ifs3.get(c);
		g_table3[i] = val;
	}
}

int main(int argc, char *argv[]) {
	std::default_random_engine rng;
	rng.seed(42);
	
	bool load = true;
	int precomputed_depth = 9;
	int table_size_bits = 24;

	if (load) {
		load_heuristic(precomputed_depth, table_size_bits);
	} else {
		auto start = std::chrono::high_resolution_clock::now();
		compute_heuristic(precomputed_depth, table_size_bits, true);
		auto stop = std::chrono::high_resolution_clock::now();
		printf("%d ms\n", std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count());
	}

	heuristic_statistic();

	Cube cube1 = get_random_cube(rng, 7);
	Cube cube2 = get_random_cube(rng, 15);
	
	IdaStarSolver dumb_solver(&dumb_heuristic);
	IdaStarSolver smart_solver(&smart_heuristic);

	printf("dumb solver:\n");
	dumb_solver.solve(cube1);
	dumb_solver.print_solution();
	printf("\n");

	printf("smart solver:\n");
	smart_solver.solve(cube1);
	smart_solver.print_solution();
	printf("\n");

	printf("smart solver, harder cube:\n");
	smart_solver.solve(cube2);
	smart_solver.print_solution();
	printf("\n");
	
	if (g_table1 != nullptr) delete[] g_table1;
	if (g_table2 != nullptr) delete[] g_table2;
	if (g_table3 != nullptr) delete[] g_table3;
	getchar();
	return 0;
}
