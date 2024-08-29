#ifndef EVAL_H
#define EVAL_H

#include "board.h"
#include "types.h"
#include "movegen.h"

#define MATERIAL_WEIGHT 1
#define MOBILITY_WEIGHT 4
#define PIECE_TABLE_WEIGHT 1

int imax(int a, int b);

int imin(int a, int b);

int material_difference();

int mobility_difference();

int square_value_table();
int evaluate();

#endif
