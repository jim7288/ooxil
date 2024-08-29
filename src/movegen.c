#include "../include/movegen.h"

uint64_t perft(int depth)
{
	struct move_list moves;
	int n_moves;
	uint64_t ret = 0;

	if (depth == 0) {
		return 1;
	}

	gen_movegen(&moves, &moves);

	for (int i = 0; i < moves.num; i++) {
		make_move(moves.list[i]);
		ret += perft(depth - 1);
		undo_move(moves.list[i]);
	}
	return ret;
}

void gen_movegen(struct move_list *ta_moves, struct move_list *qu_moves)
{
        ta_moves->num = 0;
        qu_moves->num = 0;

        // For every square on the board, computes all moves
        for (int sq = 20; sq < 100; sq++) {
                // Don't generate moves if it's not your piece
                if (!is_friendly(sq))
                        continue;

        // Use different move generation algorithm depending on the type
                switch (cboard.board[sq] % 6) {
                        case PA:
                                sgl_pawnmv(qu_moves, sq);
                                dbl_pawnmv(qu_moves, sq);
                                cap_pawnmv(ta_moves, sq);
                                break;

                        case KN:
                                knight_mv(ta_moves, qu_moves, sq);
                                break;

                        case BI:
                                diag_mv(ta_moves, qu_moves, sq);
                                break;

                        case RO:
                                orth_mv(ta_moves, qu_moves, sq);
                                break;

                        case QU:
                                diag_mv(ta_moves, qu_moves, sq);
                                orth_mv(ta_moves, qu_moves, sq);
                                break;

                        case KI:
                                king_mv(ta_moves, qu_moves, sq);
                                break;
                }
        }
}

void sliding_mv(struct move_list *ta_moves, struct move_list *qu_moves,
        square_code sq, int *dir)
{
        square_code target = sq;

        // Uses a blocker loop to generate sliding moves
        for (int i = 0; i < 4; i++) {
                target = sq + dir[i];

        // As long as the squares are empty, it's a valid move
                while (cboard.board[target] == EM) {
                        append_move(qu_moves, QUIET,
                                sq, target, cboard.board[sq], cboard.board[target]);
                        target += dir[i];
                }
        // Specifically add a cpture if there is an enemy piece at the end
                if (is_enemy(target))
                        append_move(ta_moves, CAPT,
                                sq, target, cboard.board[sq], cboard.board[target]);
        }
}

// Uses sliding move function to generate rook/queen moves
void orth_mv(struct move_list *ta_moves, struct move_list *qu_moves, square_code sq)
{
        int dir[4] = {NO, EA, SO, WE};
        sliding_mv(ta_moves, qu_moves, sq, dir);
}

// Uses sliding move function to generate bishop/queen moves
void diag_mv(struct move_list *ta_moves, struct move_list *qu_moves, square_code sq)
{
        int dir[4] = {NE, NW, SE, SW};

        sliding_mv(ta_moves, qu_moves, sq, dir);
}

void king_mv(struct move_list *ta_moves, struct move_list *qu_moves, square_code sq)
{
        int dir[8] = {NO, EA, SO, WE, NE, NW, SE, SW};
        square_code target;

        // Tests all squares of distance one and deals with them accordingly
        for (int i = 0; i < 8; i++) {
                target = sq + dir[i];
                if (cboard.board[target] == EM)
                        append_move(qu_moves, QUIET,
                                sq, target, cboard.board[sq], cboard.board[target]);
                if (is_enemy(target))
                        append_move(ta_moves, CAPT,
                                sq, target, cboard.board[sq], cboard.board[target]);
        }
}

void knight_mv(struct move_list *ta_moves, struct move_list *qu_moves, square_code sq)
{
        int dir[8] = {NNE, NNW, SSE, SSW, NEE, NWW, SEE, SWW};
        square_code target;

        // Tests all pseudolegal knight moves and deals with them accordingly
        for (int i = 0; i < 8; i++) {
                target = sq + dir[i];
                if (cboard.board[target] == EM)
                        append_move(qu_moves, QUIET,
                                sq, target, cboard.board[sq], cboard.board[target]);
                if (is_enemy(target))
                        append_move(ta_moves, CAPT,
                                sq, target, cboard.board[sq], cboard.board[target]);
        }
}

void sgl_pawnmv(struct move_list *qu_moves, square_code sq)
{
        int dir = (cboard.turn) ? NO : SO;
        square_code target = sq + dir;

        // Checks if the space ahead of the pawn is empty
        if (cboard.board[target] == EM)
                append_move(qu_moves, QUIET,
                        sq, target, cboard.board[sq], cboard.board[target]);
}

void dbl_pawnmv(struct move_list *qu_moves, square_code sq)
{
        int dir = (cboard.turn) ? NO : SO;
        int rank = (cboard.turn) ? 80 : 30;
        square_code target = sq + 2*dir;

        // Checks if the pawn is on the home rank and has space in front of it
        if (sq < rank || rank + 10 < sq )
                return;
        if (cboard.board[sq + dir] != EM)
                return;

        if (cboard.board[target] == EM)
                append_move(qu_moves, DBL_PA_PSH,
                        sq, target, cboard.board[sq], cboard.board[target]);
}

void cap_pawnmv(struct move_list *ta_moves, square_code sq)
{
        int dir[2];
        square_code target;

        // Initializes capture directions based on B/W pawn
        if (cboard.turn == W) {
                dir[0] = NE;
                dir[1] = NW;
        } else {
                dir[0] = SE;
                dir[1] = SW;
        }

        for (int i = 0; i < 2; i++) {
                target = sq + dir[i];
                if (is_enemy(target)) {
                        append_move(ta_moves, CAPT,
                                sq, target, cboard.board[sq], cboard.board[target]);
                }
        }
}

// Creates a move structure and appends it to a move list
void append_move(
        struct move_list *mv_list,
        enum move_type type_code,
        square_code from_code,
        square_code to_code,
        piece_code moved_code,
        piece_code capture_code)
{
        move_code ret = 0;

        ret |= (from_code << 20);
        ret |= (to_code << 12);
        ret |= (capture_code << 8);
        ret |= (moved_code << 4);
        ret |= (type_code << 0);

        mv_list->list[mv_list->num] = ret;
        mv_list->num++;
}
