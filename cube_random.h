#ifndef cube_random_h
#define cube_random_h

#include <random>
#include <algorithm>

#include "cube.h"

template <int length, class RNG>
Perm<length> get_random_perm(RNG rng) {
	int elements[length];
	for (int i = 0; i < length; ++i)
		elements[i] = i;
	std::shuffle(&elements[0], &elements[0] + length, rng);
	return Perm<length>(elements);
}

template <class RNG>
Cube get_random_cube(RNG rng) {
	Perm<12> edges = get_random_perm<12>(rng);
	Perm<8> corners = get_random_perm<8>(rng);
	int eges_orient[12];
	int corners_orient[8];
	int edges_orient_sum = 0;
	int corners_orient_sum = 0;
	for (int i = 0; i < 12; ++i) {
		std::uniform_int_distribution<> distr(0, 1);
		int val = distr(rng);
		eges_orient[i] = val;
		edges_orient_sum += val;
	}
	for (int i = 0; i < 8; ++i) {
		std::uniform_int_distribution<> distr(0, 2);
		int val = distr(rng);
		corners_orient[i] = val;
		corners_orient_sum += val;
	}
	// eliminate unsolvable cubes
	if (edges.parity() != corners.parity())
		edges.swap(0, 1);
	eges_orient[0] = 1 - edges_orient_sum;
	corners_orient[0] = 2 - corners_orient_sum;
	return Cube(edges, corners, eges_orient, corners_orient);
}

#endif // cube_random_h
