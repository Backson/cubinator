#ifndef metric_h
#define metric_h

#include <vector>

#include "cube.h"

#define FACE_RIGHT 0
#define FACE_LEFT  1
#define FACE_UP    2
#define FACE_DOWN  3
#define FACE_FRONT 4
#define FACE_BACK  5
#define FACE_NUM   6

class Metric {
public:
	int get_move_number() const;
	const Cube &get_move(int) const;

	static const Metric &get(int);
	static void initialize();

private:
	std::vector<Cube> moves;

	static Metric invalid;
	static Metric quarter_turn;
	static Metric half_turn;
	static bool initialized;
};

#endif // metric_h
