#ifndef SEARCH_H
#define SEARCH_H

#include "movegen.h"
#include "eval.h"
#include "board.h"
#include "types.h"

int compare_moves(const void *move_1, const void *move_2);
void sort_moves(void *moves, int items);

int lva_mvv(const void *move_1, const void *move_2);
void sort_tactical_moves(void *moves, int items);

move_code get_move(unsigned int depth);
int alpha_beta(int alpha, int beta, unsigned int depth);
int alpha_beta_recursive(int a, int b, unsigned int depth);
int quiesce(int alpha, int beta);

#endif
