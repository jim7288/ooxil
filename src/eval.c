#include "../include/eval.h"

int imax(int a, int b)
{
        return (a > b) ? a : b;
}

int imin(int a, int b)
{
        return (a < b) ? a : b;
}

// Calculates material differences on the board
int material_difference()
{
        int result = 0;

        for (int sq = 20; sq < 100; sq++) {
                switch (cboard.board[sq]) {
                        case WP:
                                result += 100;
                                break;
                        case BP:
                                result -= 100;
                                break;

                        case WN:
                                result += 300;
                                break;
                        case BN:
                                result -= 300;
                                break;
                        case WB:
                                result += 300;
                                break;

                        case BB:
                                result -= 300;
                                break;

                        case WR:
                                result += 500;
                                break;
                        case BR:
                                result -= 500;
                                break;

                        case WQ:
                                result += 900;
                                break;
                        case BQ:
                                result -= 900;
                                break;

                        case WK:
                                result += 100000;
                                break;
                        case BK:
                                result -= 100000;
                                break;
                        default:
                                break;
                }
        }

        return result;
}

// Compares the number of moves available to each side
int mobility_difference()
{
        struct move_list ta_moves;
        struct move_list qu_moves;
        enum turns turn = cboard.turn;

        int result = 0;

        cboard.turn = W;
        gen_movegen(&ta_moves, &qu_moves);
        result += (qu_moves.num + ta_moves.num);
        cboard.turn = B;
        gen_movegen(&ta_moves, &qu_moves);
        result -= (qu_moves.num + ta_moves.num);
        cboard.turn = turn;

        return result;
}

// Uses piece lookup tables to estimate a player's positional value
int square_value_table()
{
        int result = 0;

        int pawn_table[120] = {
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                0, 50, 50, 50, 50, 50, 50, 50, 50,  0,
                0, 10, 10, 20, 30, 30, 20, 10, 10,  0,
                0,  5,  5, 10, 25, 25, 10,  5,  5,  0,
                0,  0,  0,  0, 20, 20,  0,  0,  0,  0,
                0,  5, -5,-10,  0,  0,-10, -5,  5,  0,
                0,  5, 10, 10,-20,-20, 10, 10,  5,  0,
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0
        };

        int knight_table[120] = {
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                0,-50,-40,-30,-30,-30,-30,-40,-50,  0,
                0,-40,-20,  0,  0,  0,  0,-20,-40,  0,
                0,-30,  0, 10, 15, 15, 10,  0,-30,  0,
                0,-30,  5, 15, 20, 20, 15,  5,-30,  0,
                0,-30,  0, 15, 20, 20, 15,  0,-30,  0,
                0,-30,  5, 10, 15, 15, 10,  5,-30,  0,
                0,-40,-20,  0,  5,  5,  0,-20,-40,  0,
                0,-50,-40,-30,-30,-30,-30,-40,-50,  0,
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0
        };

        int bishop_table[120] = {
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                0,-20,-10,-10,-10,-10,-10,-10,-20,  0,
                0,-10,  0,  0,  0,  0,  0,  0,-10,  0,
                0,-10,  0,  5, 10, 10,  5,  0,-10,  0,
                0,-10,  5,  5, 10, 10,  5,  5,-10,  0,
                0,-10,  0, 10, 10, 10, 10,  0,-10,  0,
                0,-10, 10, 10, 10, 10, 10, 10,-10,  0,
                0,-10,  5,  0,  0,  0,  0,  5,-10,  0,
                0,-20,-10,-10,-10,-10,-10,-10,-20,  0,
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0
        };

        int rook_table[120] = {
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                0,  5, 10, 10, 10, 10, 10, 10,  5,  0,
                0, -5,  0,  0,  0,  0,  0,  0, -5,  0,
                0, -5,  0,  0,  0,  0,  0,  0, -5,  0,
                0, -5,  0,  0,  0,  0,  0,  0, -5,  0,
                0, -5,  0,  0,  0,  0,  0,  0, -5,  0,
                0, -5,  0,  0,  0,  0,  0,  0, -5,  0,
                0,  0,  0,  0,  5,  5,  0,  0,  0,  0,
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0
        };

        int queen_table[120] = {
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                0,-20,-10,-10, -5, -5,-10,-10,-20,  0,
                0,-10,  0,  0,  0,  0,  0,  0,-10,  0,
                0,-10,  0,  5,  5,  5,  5,  0,-10,  0,
                0, -5,  0,  5,  5,  5,  5,  0, -5,  0,
                0, -5,  0,  5,  5,  5,  5,  0, -5,  0,
                0,-10,  0,  5,  5,  5,  5,  0,-10,  0,
                0,-10,  0,  0,  0,  0,  0,  0,-10,  0,
                0,-20,-10,-10, -5, -5,-10,-10,-20,  0,
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0
        };

        int king_table[120] = {
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                0,-30,-40,-40,-50,-50,-40,-40,-30,  0,
                0,-30,-40,-40,-50,-50,-40,-40,-30,  0,
                0,-30,-40,-40,-50,-50,-40,-40,-30,  0,
                0,-30,-40,-40,-50,-50,-40,-40,-30,  0,
                0,-20,-30,-30,-40,-40,-30,-30,-20,  0,
                0,-10,-20,-20,-20,-20,-20,-20,-10,  0,
                0, 20, 20,  0,  0,  0,  0, 20, 20,  0,
                0, 20, 30, 10,  0,  0, 10, 30, 20,  0,
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                0,  0,  0,  0,  0,  0,  0,  0,  0,  0
        };

        for (int sq = 20; sq < 100; sq++) {
                switch (cboard.board[sq]) {
                        case WP:
                                result += pawn_table[sq];
                                break;
                        case BP:
                                result -= pawn_table[119 - sq];
                                break;

                        case WN:
                                result += knight_table[sq];
                                break;
                        case BN:
                                result -= knight_table[119 - sq];
                                break;

                        case WB:
                                result += bishop_table[sq];
                                break;
                        case BB:
                                result -= bishop_table[119 - sq];
                                break;

                        case WR:
                                result += rook_table[sq];
                                break;
                        case BR:
                                result -= rook_table[119 - sq];
                                break;

                        case WQ:
                                result += queen_table[sq];
                                break;
                        case BQ:
                                result -= queen_table[119 - sq];
                                break;

                        case WK:
                                result += king_table[sq];
                                break;
                        case BK:
                                result -= king_table[119 - sq];
                                break;

                        default:
                                break;
                }
        }

        return result;
}

// Combines all factors for a heuristic estimation of the current position
int evaluate()
{
        int score = 0;

        score += material_difference() * MATERIAL_WEIGHT;
        score += mobility_difference() * MOBILITY_WEIGHT;
        score += square_value_table() * PIECE_TABLE_WEIGHT;

        //printf("turn %d\n", cboard.turn);

        return (cboard.turn == W) ? score : -score;
}
