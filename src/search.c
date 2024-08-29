#include "../include/search.h"
int nodes_searched;
int quiet_nodes_searched;
// Compares two moves based on their score and returns an integer value

/*
int compare_moves(const void *move_1, const void *move_2)
{
        move_code m1 = *((move_code*)move_1);
        move_code m2 = *((move_code*)move_2);
        return -(m1.score - m2.score);
}

// Sorts moves from highest score to lowest score
void sort_moves(void *moves, int items)
{
        qsort(moves, items, sizeof(move_code), compare_moves);
}

*/

// Ranks captures based on the value of piece attacking and the value of capture
int lva_mvv(const void *move_1, const void *move_2)
{
        move_code m1 = *((move_code*)move_1);
        move_code m2 = *((move_code*)move_2);
        return -((capture(m1) % 6 - piece(m1) % 6) - (capture(m2) % 6 - piece(m2) % 6));
}

// Sorts moves based on the capture value and the piece value
void sort_tactical_moves(void *move_list, int items)
{
        qsort(move_list, items, sizeof(move_code), lva_mvv);
}

// Finds the best move in any position to a fixed depth
move_code get_move(unsigned int depth)
{
        unsigned int search_depth = depth;

        struct move_list moves;
        move_code best_move;

        int alpha, beta;
        int best_score, score;
        gen_movegen(&moves, &moves);

        // Creates initial scores for move ordering purposes for every move
        /*
        for (int i = 0; i < moves.num; i++) {
                if (cboard.board[moves.list[i].to] % 6 == KI)
                        moves.list[i].score = INT_MAX;
                make_move(&moves.list[i]);
                moves.list[i].score = -quiesce(INT_MAX, -INT_MAX);
                undo_move(moves.list[i]);
        }*/

        //printf("Succeeded initial scores...\n");

//        sort_moves(moves.list, moves.num);

        //printf("Succeeded initial move sort..\n");

        while (search_depth <= depth) {
                best_score = -INT_MAX;
                alpha = -INT_MAX;
                beta = INT_MAX;

                for (int i = 0; i < moves.num; i++) {

                        if (cboard.board[to(moves.list[i])] % 6 == KI)
                                return moves.list[i];

                        make_move(moves.list[i]);

                        score = -alpha_beta_recursive(-beta, -alpha, search_depth - 1);

                        alpha = imax(score, alpha);

                        if (score > best_score) {
                                best_score = score;
                                best_move = moves.list[i];
                        }

                        undo_move(moves.list[i]);
                }
                search_depth++;
        }

        printf("Best score: %d\n", best_score);

        return best_move;
}

int alpha_beta(int a, int b, unsigned int depth)
{
        //printf("alpha-beta: Entered A-B search of depth %d...\n", depth);

        struct move_list moves[MAX_SEARCH_DEPTH];
        int searched[MAX_SEARCH_DEPTH];
        int score[MAX_SEARCH_DEPTH];
        int alpha[MAX_SEARCH_DEPTH] = {a};
        int beta[MAX_SEARCH_DEPTH] = {b};

        int index = 0;

        for (int i = 0; i < MAX_SEARCH_DEPTH; i++) {
                score[i] = -INT_MAX;
                searched[i] = 0;
        }

        //("alpha-beta: Initialized all variables...\n");

        gen_movegen(&moves[0], &moves[0]);

        //printf("alpha-beta: Generated moves; %d moves available...\n", moves[0].num);

        // Stalemate if no moves remain
        if (moves[0].num == 0)
                return 0;

        /* Remake IID */ //!!!
        // Test and evaluate all moves
        while (searched[0] < moves[0].num) {
                /*Check that the king still exists
                if (cboard.board[moves[index].list[searched[index]].to] % 6 == KI) {
                        printf("Did bro actually run: %d %d\n", KI, cboard.board[moves[index].list[searched[index]].to] % 6);
                        print_move(moves[index].list[searched[index]]);
                        print_board();

                        undo_move(moves[index].list[searched[index]]);
                        print_board();
                        searched[index] = 0;
                        score[index] = INT_MAX;
                        index--;

                        score[index] = imax(score[index], -score[index + 1]);
                        alpha[index] = imax(score[index], alpha[index]);

                        if (alpha[index] > beta[index])
                                searched[index] = INT_MAX;
                }*/

                if (index >= depth || searched[index] >= moves[index].num) {
                        if (index >= depth) {
                                score[index] = quiesce(alpha[index], beta[index]);
                        }

                        index--;
                        undo_move(moves[index].list[searched[index]]); /*searched[index] gets incremented, so wrong move is undone*/
                        /*printf("undid move %d %d\n", index, searched[index]);

                        printf("Undoing\n");
                        print_move(moves[index].list[searched[index]]);
                        print_board();*/
                        searched[index]++;
                        score[index] = imax(score[index], -score[index + 1]);
                        alpha[index] = imax(score[index], alpha[index]);

                        score[index + 1] = -INT_MAX;
                        searched[index + 1] = 0;

                        if (alpha[index] > beta[index]) {
                                searched[index] = INT_MAX;
                        }
                } else {
                        if (to(cboard.board[moves[index].list[searched[index]]]) % 6 == KI) {
                                score[index] = INT_MAX;
                                alpha[index] = INT_MAX;
                                searched[index]++;
                                continue;
                        }

                        make_move(moves[index].list[searched[index]]);
                        nodes_searched++;
                        /*
                        printf("made move %d %d\n", index, searched[index]);
                        printf("Making\n");
                        print_move(moves[index].list[searched[index]]);
                        print_board();*/
                        alpha[index + 1] = -beta[index];
                        beta[index + 1] = -alpha[index];

                        index++;
                        //printf("%d erm index", index);
                        gen_movegen(&moves[index], &moves[index]);
                        //printf("alpha-beta: Increasing search depth to %d\n", index);
                } 

                /*
                for (int i = 0; i < moves.num; i++) {
                        if (cboard.board[moves.list[i].to] % 6 == KI)
                                return 100000;

                        make_move(moves.list[i]);

                        score = imax(score, -alpha_beta(-beta, -alpha, depth - 1));

                        undo_move(moves.list[i]);

                        alpha = imax(score, alpha);
                        
                        if (alpha >= beta)
                                return score;
                }*/
        }
        /*
        for (int i = 0; i < MAX_SEARCH_DEPTH; i++) {
                printf("%d ", score[i]);
        } printf("\n");*/

        //printf("alpha-beta: Exiting safely...\n");

        return score[0];
}

int alpha_beta_recursive(int alpha, int beta, unsigned int depth)
{
        //printf("alpha-beta: Entered A-B search of depth %d...\n", depth);
        if (depth == 0) {
                return quiesce(alpha, beta);
        }

        struct move_list moves;
        int score;

        score = -INT_MAX;


        gen_movegen(&moves, &moves);

        // Stalemate if no moves remain
        if (moves.num == 0)
                return 0;

        /* Remake IID */ //!!!

        // Test and evaluate all moves
        for (int i = 0; i < moves.num; i++) {
                if (cboard.board[to(moves.list[i])] % 6 == KI)
                        return INT_MAX;

                make_move(moves.list[i]);
                nodes_searched++;

                score = imax(score, -alpha_beta_recursive(-beta, -alpha, depth - 1));

                undo_move(moves.list[i]);

                alpha = imax(alpha, score);

                if (alpha > beta) {
                        return alpha;
                }
        }

        return score;
}


int quiesce(int alpha, int beta)
{
        quiet_nodes_searched++;
        int stand_pat = evaluate();
        int score;

        // Stop searching if the current position is already the best
        if (stand_pat >= beta)
                return stand_pat;

        // Stop searching if this line will never lead to a good outcome
        if (stand_pat + 200 <= alpha)
                return stand_pat;

        alpha = imax(alpha, stand_pat);

        struct move_list ta_moves, qu_moves;
        gen_movegen(&ta_moves, &qu_moves);

        sort_tactical_moves(ta_moves.list, ta_moves.num);

        // Test and return evaluations for all moves
        for (int i = 0; i < ta_moves.num; i++) {
                if (cboard.board[to(ta_moves.list[i])] % 6 == KI)
                        return INT_MAX;

                make_move(ta_moves.list[i]);

                score = -quiesce(-beta, -alpha);

                undo_move(ta_moves.list[i]);

                alpha = imax(score, alpha);

                if (score >= beta)
                        return score;
        }

        return alpha;
}
