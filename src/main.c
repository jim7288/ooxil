#include <stdio.h>
#include "../include/board.h"
#include "../include/eval.h"
#include "../include/movegen.h"
#include "../include/search.h"
#include <time.h>

int main(int argc, char *argv[])
{
        move_code move; // Variable to store move
        struct move_list moves;
        clock_t start;
        double secs = 0;

        int depth = 3;

        if (argc > 0) {
                depth = atoi(argv[1]);
        }

        new_game();
        print_board();

        nodes_searched = 0;

        for (int i = 0; i < 9; i++) {
                quiet_nodes_searched = 0;
                start = clock();

                move = get_move(depth);

                print_move(move);

                secs += ((double)clock() - start) / CLOCKS_PER_SEC;
                make_move(move);
                print_board();
        }

        printf("%d nodes @ %f nps @ %f secs\n", nodes_searched, nodes_searched / secs, secs);

        return 0;

}
