#ifndef word_h
#define word_h

#include "cube.h"

#define METRIC_QUARTER_TURN 0
#define METRIC_HALF_TURN    1

#define WORD_NUM 45

#define WORD_LR01 0
#define WORD_LR02 1
#define WORD_LR03 2

#define WORD_LR10 3
#define WORD_LR11 4
#define WORD_LR12 5
#define WORD_LR13 6

#define WORD_LR20 7
#define WORD_LR21 8
#define WORD_LR22 9
#define WORD_LR23 10

#define WORD_LR30 11
#define WORD_LR31 12
#define WORD_LR32 13
#define WORD_LR33 14

#define WORD_FB01 15
#define WORD_FB02 16
#define WORD_FB03 17

#define WORD_FB10 18
#define WORD_FB11 19
#define WORD_FB12 20
#define WORD_FB13 21

#define WORD_FB20 22
#define WORD_FB21 23
#define WORD_FB22 24
#define WORD_FB23 25

#define WORD_FB30 26
#define WORD_FB31 27
#define WORD_FB32 28
#define WORD_FB33 29

#define WORD_UD01 30
#define WORD_UD02 31
#define WORD_UD03 32

#define WORD_UD10 33
#define WORD_UD11 34
#define WORD_UD12 35
#define WORD_UD13 36

#define WORD_UD20 37
#define WORD_UD21 38
#define WORD_UD22 39
#define WORD_UD23 40

#define WORD_UD30 41
#define WORD_UD31 42
#define WORD_UD32 43
#define WORD_UD33 44

int get_word_cost(int i, int metric);
Cube get_word_cube(int i);
bool can_words_be_switched(int i, int j);

#endif // word_h
