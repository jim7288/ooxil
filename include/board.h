#ifndef BOARD_H
#define BOARD_H

#include "types.h"
#include "board.h"

extern struct board cboard;

void new_game();

char *algebraic(move_code move, char *string);

bool is_mate();
int threats(square_code sq);

void print_move(move_code move);
void print_board();

bool is_enemy(square_code sq);
bool is_friendly(square_code sq);

void make_move(move_code move);
void undo_move(move_code move);

square_code from(move_code move);

square_code to(move_code move);

piece_code capture(move_code move);

piece_code piece(move_code move);

enum move_type type(move_code move);

#endif
