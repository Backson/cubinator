#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "permut.h"
#include "cube.h"
#include "factorial.h"

template<int length>
void print_perm(Perm<length> perm) {
	for (int i = 0; i < length; i++)
		printf("%d ", perm.element(i));
	printf("\n");
}

void print_cube(const Cube& cube) {
	//printf("e ");
	for (int i = 0; i < 12; i++)
		printf("%c%1X ", cube.edge_orients()[i] ? '-' : '+', cube.edges().element(i));
	
	printf(" :  ");
	
	for (int i = 0; i < 8; i++) {
		static const char cs[] = {'.','+','-'};
		char c = cs[cube.corner_orients()[i]];
		printf("%c%1X ", c, cube.corners().element(i));
	}
	
	printf("\n");
}

void draw_cube(const Cube& cube) {
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
	
	// up
	auto stone1 = corner_stones[cube.corners().element(7)][(3-cube.corner_orients()[7]) % 3];
	auto stone2 = edge_stones[cube.edges().element(10)][cube.edge_orients()[10]];
	auto stone3 = corner_stones[cube.corners().element(6)][(3-cube.corner_orients()[6]) % 3];
	printf(" %c %c %c \n", stone1, stone2, stone3);
	
	stone1 = edge_stones[cube.edges().element(11)][cube.edge_orients()[11]];
	stone2 = edge_stones[cube.edges().element(9)][cube.edge_orients()[9]];
	printf(" %c y %c \n", stone1, stone2);
	
	stone1 = corner_stones[cube.corners().element(4)][(3-cube.corner_orients()[4]) % 3];
	stone2 = edge_stones[cube.edges().element(8)][cube.edge_orients()[8]];
	stone3 = corner_stones[cube.corners().element(5)][(3-cube.corner_orients()[5]) % 3];
	printf(" %c %c %c \n", stone1, stone2, stone3);
	
	printf("+-----+\n");
}


int main(int argc, char *argv[])
{
	srand(time(NULL));
	
	static const Cube& r = Cube::TURN_RIGHT;
	static const Cube& l = Cube::TURN_LEFT;
	
	static const Cube& u = Cube::TURN_UP;
	static const Cube& d = Cube::TURN_DOWN;
	
	static const Cube& f = Cube::TURN_FRONT;
	static const Cube& b = Cube::TURN_BACK;
	
	auto tperm = r+u-r-u-r+f+r+r-u-r-u+r+u-r-f;
	//print_cube(tperm);
	//print_cube(tperm+tperm);
	
	//print_cube((Cube::parse("R U R' U'"))*6);
	
	Perm<6> perm;
	print_perm(perm);
	printf("%d\n", perm.parity());
	
	for (int i = 0; i < 20; i++) {
		int a = rand() % 6;
		int b = rand() % 5;
		if (b >= a) b++;
		
		perm.swap(a, b);
		print_perm(perm);
		printf("%d\n", perm.parity());
	}
	
	return 0;
}
