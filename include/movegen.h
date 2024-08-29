#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "eval.h"
#include "board.h"
#include "types.h"

void gen_movegen(struct move_list *ta_moves, struct move_list *qu_moves);

void sliding_mv(
        struct move_list *ta_moves,
        struct move_list *qu_moves,
        square_code sq, int *dir);

void orth_mv(struct move_list *ta_moves,
        struct move_list *qu_moves, square_code sq);

void diag_mv(struct move_list *ta_moves,
        struct move_list *qu_moves, square_code sq);

void king_mv(struct move_list *ta_moves,
        struct move_list *qu_moves, square_code sq);

void knight_mv(struct move_list *ta_moves,
        struct move_list *qu_moves, square_code sq);

void sgl_pawnmv(struct move_list *qu_moves, square_code sq);

void dbl_pawnmv(struct move_list *qu_moves, square_code sq);

void cap_pawnmv(struct move_list *ta_moves, square_code sq);

void append_move(
        struct move_list *mv_list,
        enum move_type type_code,
        square_code from_code,
        square_code to_code,
        piece_code moved_code,
        piece_code capture_code);
#endif
