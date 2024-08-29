#include "../include/board.h"

struct board cboard;

bool is_mate();
int threats(square_code sq);

// Prints moves in a human-readable way
void print_move(move_code move)
{
        printf("type: %d\ncapture: %d\n", type(move), capture(move));
        printf("piece: %d\n", piece(move));
        printf("from: %d\nto: %d\n", from(move), to(move));
}

// Initializs the board for a new game
void new_game()
{
        enum piece_value empty_board[120] = {
                IV, IV, IV, IV, IV, IV, IV, IV, IV, IV,
                IV, IV, IV, IV, IV, IV, IV, IV, IV, IV,
                IV, BR, BN, BB, BQ, BK, BB, BN, BR, IV,
                IV, BP, BP, BP, BP, BP, BP, BP, BP, IV,
                IV, EM, EM, EM, EM, EM, EM, EM, EM, IV,
                IV, EM, EM, EM, EM, EM, EM, EM, EM, IV,
                IV, EM, EM, EM, EM, EM, EM, EM, EM, IV,
                IV, EM, EM, EM, EM, EM, EM, EM, EM, IV,
                IV, WP, WP, WP, WP, WP, WP, WP, WP, IV,
                IV, WR, WN, WB, WQ, WK, WB, WN, WR, IV,
                IV, IV, IV, IV, IV, IV, IV, IV, IV, IV,
                IV, IV, IV, IV, IV, IV, IV, IV, IV, IV,
        };

    // fill board
        for (int i = 0; i < 120; i++) {
                cboard.board[i] = empty_board[i];
        }

    // Set intial values
        cboard.fifty_move_counter = 0;
        cboard.time_remaining = true;
        cboard.turn = W;
        cboard.king[W] = 95;
        cboard.king[B] = 25;
}

// Prints board in a human-readable way
void print_board()
{
        char piece = '-';
        printf("\n");
        for(int i = 0; i < 120; i += 10) {
                for (int j = 0; j < 10; j++) {
                        switch (cboard.board[i + j]) {
                                case WP:
                                        piece = 'P';
                                        break;
                                case BP:
                                        piece = 'p';
                                        break;
                                case WN:
                                        piece = 'N';
                                        break;
                                case BN:
                                        piece = 'n';
                                        break;
                                case WB:
                                        piece = 'B';
                                        break;
                                case BB:
                                        piece = 'b';
                                        break;
                                case WR:
                                        piece = 'R';
                                        break;
                                case BR:
                                        piece = 'r';
                                        break;
                                case WQ:
                                        piece = 'Q';
                                        break;
                                case BQ:
                                        piece = 'q';
                                        break;
                                case WK:
                                        piece = 'K';
                                        break;
                                case BK:
                                        piece = 'k';
                                        break;
                                case EM:
                                        piece = '.';
                                        break;
                                case IV:
                                        piece = '-';
                                        break;
                        }
                        printf(" %c ", piece);
                }
                printf("\n");
        }
        printf("\n");
}

// Checks if a square as an enemy piece
bool is_enemy(square_code sq)
{
        if (cboard.board[sq] == IV || cboard.board[sq] == EM)
                return false;

        return cboard.turn ? (cboard.board[sq] > 5) : (cboard.board[sq] <= 5);
}

// Checks if a square has a friendly piece
bool is_friendly(square_code sq)
{
        if (cboard.board[sq] == IV || cboard.board[sq] == EM)
                return false;

        return cboard.turn ? (cboard.board[sq] <= 5) : (cboard.board[sq] > 5);
}

// Makes a move based on the type of move
void make_move(move_code move)
{
        switch (type(move)) {
                case (DBL_PA_PSH):
                case (QUIET):
                case (CAPT):
                        cboard.board[to(move)] = piece(move);
                        cboard.board[from(move)] = EM;
                        break;
                case (KS_CASTLE):
                        break;
                case (QS_CASTLE):
                        break;
                case (EP_CAPT):
                        break;
                case (KN_PRM):
                        break;
                case (BI_PRM):
                        break;
                case (RO_PRM):
                        break;
                case (QU_PRM):
                        break;
                case (KN_CAPT_PRM):
                        break;
                case (BI_CAPT_PRM):
                        break;
                case (RO_CAPT_PRM):
                        break;
                case (QU_CAPT_PRM):
                        break;
        }

        cboard.turn = !cboard.turn;
}

// Undoes a move based on the type of move
void undo_move(move_code move)
{
        switch (type(move)) {
                case (DBL_PA_PSH):
                case (QUIET):
                        cboard.board[from(move)] = piece(move);
                        cboard.board[to(move)] = EM;
                        break;
                case (CAPT):
                        cboard.board[from(move)] = piece(move);
                        cboard.board[to(move)] = capture(move);
                        break;
                case (KS_CASTLE):
                        break;
                case (QS_CASTLE):
                        break;
                case (EP_CAPT):
                        break;
                case (KN_PRM):
                        break;
                case (BI_PRM):
                        break;
                case (RO_PRM):
                        break;
                case (QU_PRM):
                        break;
                case (KN_CAPT_PRM):
                        break;
                case (BI_CAPT_PRM):
                        break;
                case (RO_CAPT_PRM):
                        break;
                case (QU_CAPT_PRM):
                        break;
        }

        cboard.turn = !cboard.turn;
}

square_code from(move_code move) {
        return (move >> 20) & 0xFF;
}

square_code to(move_code move) {
        return (move >> 12) & 0xFF;
}

piece_code capture(move_code move) {
        return (move >> 8) & 0xF;
}

piece_code piece(move_code move) {
        return (move >> 4) & 0xF;
}

enum move_type type(move_code move) {
        return (move >> 0) & 0xF;
}
