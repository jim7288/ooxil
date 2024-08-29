#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <limits.h>
#include <float.h>
#include <string.h>

#define MAX_SEARCH_DEPTH 16
#define IV_SQUARE 255

extern int quiet_nodes_searched;
extern int nodes_searched;

enum turns {
        B, W
};

// Creates enumeration values for easy piece indexing
enum piece_index {
        PA, KN, BI, RO, QU, KI
};

// Enumerates directions on a chess board
enum direction {
        NO = -10, EA = 1, SO = 10, WE = -1,
        NE = -9, SE = 11, NW = -11, SW = 9,
        NNE = -19, NEE = -8, SEE = 12, SSE = 21,
        SSW = 19, SWW = 8, NWW = -12, NNW = -21
};

// Enumerates piece codes
enum piece_value {
        WP, WN, WB, WR, WQ, WK,
        BP, BN, BB, BR, BQ, BK,
        EM = 14, IV = 15
};

// Encodes move types as integers
enum move_type {
        QUIET, // 0
        KS_CASTLE, // 1
        QS_CASTLE, // 2
        DBL_PA_PSH, // 3
        CAPT, // 4
        EP_CAPT, // 5
        KN_PRM, BI_PRM,
        RO_PRM, QU_PRM,
        KN_CAPT_PRM, BI_CAPT_PRM,
        RO_CAPT_PRM, QU_CAPT_PRM
};

// Creates aliases for various types
typedef uint8_t piece_code;
typedef uint8_t square_code;

/*
 * Defines move format
 * --8----8------4-----4-------4---
 * from | to | capt | moved | code
 */
typedef uint32_t move_code;

// Defines the array used for move storage
struct move_list {
        int num;
        move_code list[256];
};

// Defines the board representation
struct board {
        uint8_t fifty_move_counter;
        uint8_t move_count;
        square_code ep_square;
        bool castling_rights[2][2];
        bool time_remaining;

        enum piece_value board[120];
        square_code king[2];

        enum turns turn;
};
#endif
