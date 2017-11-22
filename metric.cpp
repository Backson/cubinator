#include "metric.h"

#include "word.h"

int Metric::get_move_number() const {
	return moves.size();
}

const Cube &Metric::get_move(int i) const {
	return moves[i];
}

bool Metric::initialized = false;

Metric Metric::invalid;
Metric Metric::quarter_turn;
Metric Metric::half_turn;

void Metric::initialize() {
	invalid.moves.clear();

	quarter_turn.moves.clear();
	quarter_turn.moves.push_back(Cube::TURN_RIGHT);
	quarter_turn.moves.push_back(Cube::TURN_LEFT);
	quarter_turn.moves.push_back(Cube::TURN_UP);
	quarter_turn.moves.push_back(Cube::TURN_DOWN);
	quarter_turn.moves.push_back(Cube::TURN_FRONT);
	quarter_turn.moves.push_back(Cube::TURN_BACK);
	quarter_turn.moves.push_back(-Cube::TURN_RIGHT);
	quarter_turn.moves.push_back(-Cube::TURN_LEFT);
	quarter_turn.moves.push_back(-Cube::TURN_UP);
	quarter_turn.moves.push_back(-Cube::TURN_DOWN);
	quarter_turn.moves.push_back(-Cube::TURN_FRONT);
	quarter_turn.moves.push_back(-Cube::TURN_BACK);

	half_turn.moves.clear();
	half_turn.moves.push_back(Cube::TURN_RIGHT);
	half_turn.moves.push_back(Cube::TURN_LEFT);
	half_turn.moves.push_back(Cube::TURN_UP);
	half_turn.moves.push_back(Cube::TURN_DOWN);
	half_turn.moves.push_back(Cube::TURN_FRONT);
	half_turn.moves.push_back(Cube::TURN_BACK);
	half_turn.moves.push_back(-Cube::TURN_RIGHT);
	half_turn.moves.push_back(-Cube::TURN_LEFT);
	half_turn.moves.push_back(-Cube::TURN_UP);
	half_turn.moves.push_back(-Cube::TURN_DOWN);
	half_turn.moves.push_back(-Cube::TURN_FRONT);
	half_turn.moves.push_back(-Cube::TURN_BACK);
	half_turn.moves.push_back(Cube::TURN_RIGHT * 2);
	half_turn.moves.push_back(Cube::TURN_LEFT * 2);
	half_turn.moves.push_back(Cube::TURN_UP * 2);
	half_turn.moves.push_back(Cube::TURN_DOWN * 2);
	half_turn.moves.push_back(Cube::TURN_FRONT * 2);
	half_turn.moves.push_back(Cube::TURN_BACK * 2);
}

const Metric &Metric::get(int metric) {
	if (!initialized) {
		initialize();
		initialized = true;
	}

	switch (metric) {
	case METRIC_QUARTER_TURN: return quarter_turn;
	case METRIC_HALF_TURN: return half_turn;
	default: return invalid;
	}
}
