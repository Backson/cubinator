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
#include "heuristic.h"

#define HEURISTIC_CORNER_TABLE_DEPTH 9
#define HEURISTIC_CORNER_TABLE_BITS 24
#define HEURISTIC_EDGES_TABLE_DEPTH 9
#define HEURISTIC_EDGES_TABLE_BITS 22

std::default_random_engine rng;

static Cube cube_superflip;
static Cube cube_checkerboard;
static Cube cube_four_spots;
static Cube cube_six_spots;
static Cube cube_facing_checkerboard;
static Cube cube_cross;
static Cube cube_cross2;

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
	return 1;
}

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

	for (int i = 0; i < 12; ++i){
		int element = cube.edges().element(i);
		int orient = cube.edge_orients()[i];
		if (0 <= element && element < 6) {
			hash = element + (hash << 6) + (hash << 16) - hash;
			hash = orient + (hash << 6) + (hash << 16) - hash;
			hash = (hash << 1) | (hash >> 31);
		}
		else {
			hash = (2 * i + 13) + (hash << 6) + (hash << 16) - hash;
			hash = (hash << 1) | (hash >> 31);
		}
	}
	return hash;
};

unsigned int hash_function_lower_edges(const Cube &cube) {
	unsigned int hash = 0xb474a585;

	for (int i = 0; i < 12; ++i){
		int element = cube.edges().element(i);
		int orient = cube.edge_orients()[i];
		if (6 <= element && element < 12) {
			hash = element + (hash << 6) + (hash << 16) - hash;
			hash = orient + (hash << 6) + (hash << 16) - hash;
			hash = (hash << 1) | (hash >> 31);
		}
		else {
			hash = (2 * i + 13) + (hash << 6) + (hash << 16) - hash;
			hash = (hash << 1) | (hash >> 31);
		}
	}
	return hash;
};

int smart_heuristic1(const Cube &cube) {
	static HashTableHeuristic h("corners",
		hash_function_corners, HEURISTIC_CORNER_TABLE_DEPTH, HEURISTIC_CORNER_TABLE_BITS);
	int result = h(cube);
	return result;
}

int smart_heuristic2(const Cube &cube) {
	static HashTableHeuristic h("uedges",
		hash_function_upper_edges, HEURISTIC_EDGES_TABLE_DEPTH, HEURISTIC_EDGES_TABLE_BITS);
	int result = h(cube);
	return result;
}

int smart_heuristic3(const Cube &cube) {
	static HashTableHeuristic h("ledges",
		hash_function_lower_edges, HEURISTIC_EDGES_TABLE_DEPTH, HEURISTIC_EDGES_TABLE_BITS);
	int result = h(cube);
	return result;
}

int smart_heuristic(const Cube &cube) {
	if (cube == Cube::TURN_IDENTITY)
		return 0;
	if (cube == cube_superflip)
		return 24;
	int result1 = smart_heuristic1(cube);
	int result2 = smart_heuristic2(cube);
	int result3 = smart_heuristic3(cube);
	return std::max(std::max(result1, result2), result3);
}

void heuristic_statistic() {
	int histo[30*4];
	for (int i = 0; i < 30*4; ++i)
		histo[i] = 0;

	std::default_random_engine rng;
	rng.seed(53);
	for (int i = 0; i < 1000000; ++i) {
		Cube cube = get_random_cube(rng);
		histo[smart_heuristic1(cube) + 30 * 0]++;
		histo[smart_heuristic2(cube) + 30 * 1]++;
		histo[smart_heuristic3(cube) + 30 * 2]++;
		histo[smart_heuristic(cube) + 30 * 3]++;
	}

	// print histogram of random scrambles
	printf("corners, uedges, ledges, total\n");
	for (int i = 0; i <= 20; ++i)
		printf("%2d: %7d %7d %7d %7d\n",
			i, histo[i + 30 * 0], histo[i + 30 * 1], histo[i + 30 * 2], histo[i + 30 * 3]);
	printf("\n");
}

Cube conjugate(const Cube &cube, const Cube &other) {
	return other + cube - other;
}

void test_solver() {
	Cube cube1 = get_random_cube(rng, 5);
	Cube cube2 = get_random_cube(rng, 17);

	IdaStarSolver dumb_solver(&dumb_heuristic);
	IdaStarSolver smart_solver(&smart_heuristic);

	dumb_solver.set_log();
	smart_solver.set_log();

	printf("dumb solver:\n");
	dumb_solver.solve(cube1);
	dumb_solver.print_solution();
	printf("\n");
	if (cube1 + dumb_solver.get_solution() == Cube::TURN_IDENTITY) {
		printf("Solution ok.\n");
	}
	else {
		printf("Solution not ok.\n");
	}
	printf("\n");

	printf("smart solver:\n");
	smart_solver.solve(cube1);
	smart_solver.print_solution();
	printf("\n");
	if (cube1 + smart_solver.get_solution() == Cube::TURN_IDENTITY) {
		printf("Solution ok.\n");
	}
	else {
		printf("Solution not ok.\n");
	}
	printf("\n");

	printf("smart solver, harder cube:\n");
	smart_solver.solve(cube2);
	smart_solver.print_solution();
	printf("\n");
	if (cube2 + smart_solver.get_solution() == Cube::TURN_IDENTITY) {
		printf("Solution ok.\n");
	}
	else {
		printf("Solution not ok.\n");
	}
	printf("\n");
}

void test_symmetry() {
	int counter_solvable = 0;
	for (int i = 0; i < 1000000; ++i) {
		Cube cube = get_random_cube(rng);
		if (cube.solvable())
			counter_solvable++;
	}
	printf("Randoms: %d solvable\n", counter_solvable);

	cube_superflip = Cube::parse("U R2 F B R B2 R U2 L B2 R U' D' R2 F R' L B2 U2 F2");
	cube_checkerboard = Cube::parse("F B2 R' D2 B R U D' R L' D' F' R2 D F2 B'");
	cube_four_spots = Cube::parse("F2 B2 U D' R2 L2 U D'");
	cube_six_spots = Cube::parse("U D' R L' F B' U D'");
	cube_facing_checkerboard = Cube::parse("U2 F2 U2 F2 B2 U2 F2 D2");
	cube_cross = Cube::parse("U F B' L2 U2 L2 F' B U2 L2 U");
	cube_cross2 = Cube(ExtendedCube::TURN_Z + ExtendedCube(cube_cross) - ExtendedCube::TURN_Z);

	Perm<12> edges;
	Perm<8> corners;
	int edge_orients[] = { 0,0,0,0,0,0,0,0,0,0,0,0 };
	int corner_orients[] = { 0,0,0,0,0,0,0,0 };

	edges = Perm<12>();
	edges.swap(C_UL, C_UR);
	edges.swap(C_DL, C_DR);
	edges.swap(C_FL, C_FR);
	edges.swap(C_BL, C_BR);
	corners = Perm<8>();
	corners.swap(C_ULF, C_UFR);
	corners.swap(C_UBL, C_URB);
	corners.swap(C_DFL, C_DRF);
	corners.swap(C_DLB, C_DBR);
	Cube cube_mirror(edges, corners, edge_orients, corner_orients);

	draw_cube(cube_mirror);

	printf("Mirror: %s\n", (cube_mirror.solvable() ? "solvable" : "not solvable"));
	printf("Identity: %s\n", (Cube::TURN_IDENTITY.solvable() ? "solvable" : "not solvable"));
	printf("Left: %s\n", (Cube::TURN_LEFT.solvable() ? "solvable" : "not solvable"));
	printf("Right: %s\n", (Cube::TURN_RIGHT.solvable() ? "solvable" : "not solvable"));
	printf("Up: %s\n", (Cube::TURN_UP.solvable() ? "solvable" : "not solvable"));
	printf("Down: %s\n", (Cube::TURN_DOWN.solvable() ? "solvable" : "not solvable"));
	printf("Superflip: %s\n", (cube_superflip.solvable() ? "solvable" : "not solvable"));

	Cube cube_symmetry = cube_mirror;

	draw_cube(cube_symmetry);

	printf("Symmetry: %s\n", (cube_symmetry.solvable() ? "solvable" : "not solvable"));

	for (int i = 0; i < 12; ++i) {
		Cube cube = conjugate(Cube::Metric::get(i), cube_symmetry);
		for (int j = 0; j < 12; ++j) {
			if (cube == Cube::Metric::get(j)) {
				printf("1");
			}
			else {
				printf("0");
			}
		}
		printf("\n");
	}

	int counter_symmetry = 0;
	for (int i = 0; i < 1000000; ++i) {
		Cube cube = get_random_cube(rng);
		if (cube_symmetry + cube == cube + cube_symmetry) {
			counter_symmetry++;
		}
	}
	printf("Symmetry: %d\n", counter_symmetry);
}

int main(int argc, char *argv[]) {
	rng.seed(42);

	smart_heuristic(Cube::TURN_IDENTITY);
	heuristic_statistic();

	//test_symmetry();
	test_solver();

	getchar();
	return 0;
}
