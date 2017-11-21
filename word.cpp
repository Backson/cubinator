#include "word.h"

static Cube words[WORD_NUM];
static bool initialized = false;

static Cube init_cube(int i) {
	Cube result = Cube::TURN_IDENTITY;
	switch (i) {
	case WORD_LR01:
		result += Cube::TURN_LEFT * 0 + Cube::TURN_RIGHT * 1;
		break;
	case WORD_LR02:
		result += Cube::TURN_LEFT * 0 + Cube::TURN_RIGHT * 2;
		break;
	case WORD_LR03:
		result += Cube::TURN_LEFT * 0 + Cube::TURN_RIGHT * 3;
		break;

	case WORD_LR10:
		result += Cube::TURN_LEFT * 1 + Cube::TURN_RIGHT * 0;
		break;
	case WORD_LR11:
		result += Cube::TURN_LEFT * 1 + Cube::TURN_RIGHT * 1;
		break;
	case WORD_LR12:
		result += Cube::TURN_LEFT * 1 + Cube::TURN_RIGHT * 2;
		break;
	case WORD_LR13:
		result += Cube::TURN_LEFT * 1 + Cube::TURN_RIGHT * 3;
		break;

	case WORD_LR20:
		result += Cube::TURN_LEFT * 2 + Cube::TURN_RIGHT * 0;
		break;
	case WORD_LR21:
		result += Cube::TURN_LEFT * 2 + Cube::TURN_RIGHT * 1;
		break;
	case WORD_LR22:
		result += Cube::TURN_LEFT * 2 + Cube::TURN_RIGHT * 2;
		break;
	case WORD_LR23:
		result += Cube::TURN_LEFT * 2 + Cube::TURN_RIGHT * 3;
		break;

	case WORD_LR30:
		result += Cube::TURN_LEFT * 3 + Cube::TURN_RIGHT * 0;
		break;
	case WORD_LR31:
		result += Cube::TURN_LEFT * 3 + Cube::TURN_RIGHT * 1;
		break;
	case WORD_LR32:
		result += Cube::TURN_LEFT * 3 + Cube::TURN_RIGHT * 2;
		break;
	case WORD_LR33:
		result += Cube::TURN_LEFT * 3 + Cube::TURN_RIGHT * 3;
		break;

	case WORD_FB01:
		result += Cube::TURN_FRONT * 0 + Cube::TURN_BACK * 1;
		break;
	case WORD_FB02:
		result += Cube::TURN_FRONT * 0 + Cube::TURN_BACK * 2;
		break;
	case WORD_FB03:
		result += Cube::TURN_FRONT * 0 + Cube::TURN_BACK * 3;
		break;

	case WORD_FB10:
		result += Cube::TURN_FRONT * 1 + Cube::TURN_BACK * 0;
		break;
	case WORD_FB11:
		result += Cube::TURN_FRONT * 1 + Cube::TURN_BACK * 1;
		break;
	case WORD_FB12:
		result += Cube::TURN_FRONT * 1 + Cube::TURN_BACK * 2;
		break;
	case WORD_FB13:
		result += Cube::TURN_FRONT * 1 + Cube::TURN_BACK * 3;
		break;

	case WORD_FB20:
		result += Cube::TURN_FRONT * 2 + Cube::TURN_BACK * 0;
		break;
	case WORD_FB21:
		result += Cube::TURN_FRONT * 2 + Cube::TURN_BACK * 1;
		break;
	case WORD_FB22:
		result += Cube::TURN_FRONT * 2 + Cube::TURN_BACK * 2;
		break;
	case WORD_FB23:
		result += Cube::TURN_FRONT * 2 + Cube::TURN_BACK * 3;
		break;

	case WORD_FB30:
		result += Cube::TURN_FRONT * 3 + Cube::TURN_BACK * 0;
		break;
	case WORD_FB31:
		result += Cube::TURN_FRONT * 3 + Cube::TURN_BACK * 1;
		break;
	case WORD_FB32:
		result += Cube::TURN_FRONT * 3 + Cube::TURN_BACK * 2;
		break;
	case WORD_FB33:
		result += Cube::TURN_FRONT * 3 + Cube::TURN_BACK * 3;
		break;

	case WORD_UD01:
		result += Cube::TURN_UP * 0 + Cube::TURN_DOWN * 1;
		break;
	case WORD_UD02:
		result += Cube::TURN_UP * 0 + Cube::TURN_DOWN * 2;
		break;
	case WORD_UD03:
		result += Cube::TURN_UP * 0 + Cube::TURN_DOWN * 3;
		break;

	case WORD_UD10:
		result += Cube::TURN_UP * 1 + Cube::TURN_DOWN * 0;
		break;
	case WORD_UD11:
		result += Cube::TURN_UP * 1 + Cube::TURN_DOWN * 1;
		break;
	case WORD_UD12:
		result += Cube::TURN_UP * 1 + Cube::TURN_DOWN * 2;
		break;
	case WORD_UD13:
		result += Cube::TURN_UP * 1 + Cube::TURN_DOWN * 3;
		break;

	case WORD_UD20:
		result += Cube::TURN_UP * 2 + Cube::TURN_DOWN * 0;
		break;
	case WORD_UD21:
		result += Cube::TURN_UP * 2 + Cube::TURN_DOWN * 1;
		break;
	case WORD_UD22:
		result += Cube::TURN_UP * 2 + Cube::TURN_DOWN * 2;
		break;
	case WORD_UD23:
		result += Cube::TURN_UP * 2 + Cube::TURN_DOWN * 3;
		break;

	case WORD_UD30:
		result += Cube::TURN_UP * 3 + Cube::TURN_DOWN * 0;
		break;
	case WORD_UD31:
		result += Cube::TURN_UP * 3 + Cube::TURN_DOWN * 1;
		break;
	case WORD_UD32:
		result += Cube::TURN_UP * 3 + Cube::TURN_DOWN * 2;
		break;
	case WORD_UD33:
		result += Cube::TURN_UP * 3 + Cube::TURN_DOWN * 3;
		break;
	}
	return result;
}

static void initialize() {
	for (int i = 0; i < WORD_NUM; ++i) {
		words[i] = init_cube(i);
	}
	initialized = true;
}

Cube get_word_cube(int i) {
	if (!initialized) {
		initialize();
	}
	return words[i];
}

int get_word_cost(int i, int metric) {
	static const int table[][WORD_NUM] = {
		{
			1, 2, 1, 1, 2, 3, 2, 2, 3, 4, 3, 1, 2, 3, 2,
			1, 2, 1, 1, 2, 3, 2, 2, 3, 4, 3, 1, 2, 3, 2,
			1, 2, 1, 1, 2, 3, 2, 2, 3, 4, 3, 1, 2, 3, 2,
		},{
			1, 1, 1, 1, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2,
			1, 1, 1, 1, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2,
			1, 1, 1, 1, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2,
		}
	};
	return table[metric][i];
}

// checks whether two words can come right after each other
bool can_words_be_switched(int i, int j) {
	if (i < 0 || j < 0)
		return false;
	const int CATEGORY_SIZE = WORD_NUM / 3;
	return i / CATEGORY_SIZE == j / CATEGORY_SIZE;
}
