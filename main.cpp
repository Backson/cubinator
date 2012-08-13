#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "permut.h"
#include "cube_ex.h"

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

void print_cube(const ExtendedCube& cube) {
	//printf("e ");
	for (int i = 0; i < 12; i++)
		printf("%c%1X ", cube.edge_orients()[i] ? '-' : '+', cube.edges().element(i));
	
	printf(": ");
	
	for (int i = 0; i < 8; i++) {
		static const char cs[] = {'.','+','-'};
		char c = cs[cube.corner_orients()[i]];
		printf("%c%1X ", c, cube.corners().element(i));
	}
	
	printf(": ");
	
	for (int i = 0; i < 6; i++)
		printf("%1X ", cube.middles().element(i));
	
	printf("\n");
}

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
	
	auto stone1 = corner_stones[cube.corners().element(7)][(3-cube.corner_orients()[7]) % 3];
	auto stone2 = edge_stones[cube.edges().element(10)][cube.edge_orients()[10]];
	auto stone3 = corner_stones[cube.corners().element(6)][(3-cube.corner_orients()[6]) % 3];
	printf(" %c %c %c \n", stone1, stone2, stone3);
	
	stone1 = edge_stones[cube.edges().element(11)][cube.edge_orients()[11]];
	stone2 = middle_stones[cube.middles().element(0)];
	stone3 = edge_stones[cube.edges().element(9)][cube.edge_orients()[9]];
	printf(" %c %c %c \n", stone1, stone2, stone3);
	
	stone1 = corner_stones[cube.corners().element(4)][(3-cube.corner_orients()[4]) % 3];
	stone2 = edge_stones[cube.edges().element(8)][cube.edge_orients()[8]];
	stone3 = corner_stones[cube.corners().element(5)][(3-cube.corner_orients()[5]) % 3];
	printf(" %c %c %c \n", stone1, stone2, stone3);
}

void draw_cube(const ExtendedCube& cube) {
	const char* side_names[6] = {
	"UP", "DOWN", "FRONT", "RIGHT", "BACK", "LEFT"};
	/*static const ExtendedCube::ExtendedCube turns[6] = {
		ExtendedCube::TURN_IDENTITY,
		ExtendedCube::TURN_X*2,
		ExtendedCube::TURN_X*(-1),
		ExtendedCube::TURN_Z*(-1),
		ExtendedCube::TURN_Z*(-1),
		ExtendedCube::TURN_Z*(-1)
	};*/
	
	const ExtendedCube::ExtendedCube turns[6] = {
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

int main(int argc, char *argv[])
{
	srand(time(NULL));
	
	static const Cube& r = Cube::TURN_RIGHT;
	static const Cube& l = Cube::TURN_LEFT;
	
	static const Cube& u = Cube::TURN_UP;
	static const Cube& d = Cube::TURN_DOWN;
	
	static const Cube& f = Cube::TURN_FRONT;
	static const Cube& b = Cube::TURN_BACK;
	
	//auto tperm = r+u-r-u-r+f+r+r-u-r-u+r+u-r-f;
	//draw_cube(tperm);
	char* zperm_string = "M2 U' M2 U' M' U2 M2 U2 M' U2";
	draw_cube(ExtendedCube::parse(zperm_string));
	
	draw_cube((Cube::parse("B2  R'  D2  U'  F  D2  F'  U2  D  L2  R'  U  B'  L2  F  U2  L2  U  F  B'  D2  B2  L2  B'  R")));
	
	draw_cube(ExtendedCube());
	
	printf("%i", Cube()==Cube());
	
	
	return 0;
}
