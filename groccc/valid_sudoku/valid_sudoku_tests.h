#ifndef VALID_SUDOKU_TESTS_H
#define VALID_SUDOKU_TESTS_H

#include <stdbool.h>

#include "utility/test_case_generator.h"

struct Valid_sudoku_input
{
    int board[9][9];
};

struct Valid_sudoku_output
{
    bool is_valid;
};

TCG_tests_begin(valid_sudoku_tests,
                struct Valid_sudoku_input,
                struct Valid_sudoku_output)
TCG_test_case("valid board", {
    .input = {
        .board = {
            {5,3,0, 0,7,0, 0,0,0},
            {6,0,0, 1,9,5, 0,0,0},
            {0,9,8, 0,0,0, 0,6,0},

            {8,0,0, 0,6,0, 0,0,3},
            {4,0,0, 8,0,3, 0,0,1},
            {7,0,0, 0,2,0, 0,0,6},

            {0,6,0, 0,0,0, 2,8,0},
            {0,0,0, 4,1,9, 0,0,5},
            {0,0,0, 0,8,0, 0,7,9},
        },
    },
    .output = {
        .is_valid = true,
    },
})
TCG_test_case("invalid box", {
    .input = {
        .board = {
            {5,3,0, 0,7,0, 0,0,0},
            {6,0,0, 1,9,5, 0,0,0},
            {0,9,5, 0,0,0, 0,6,0}, /* Two 5's in same box. */

            {8,0,0, 0,6,0, 0,0,3},
            {4,0,0, 8,0,3, 0,0,1},
            {7,0,0, 0,2,0, 0,0,6},

            {0,6,0, 0,0,0, 2,8,0},
            {0,0,0, 4,1,9, 0,0,5},
            {0,0,0, 0,8,0, 0,7,9},
        },
    },
    .output = {
        .is_valid = false,
    },
})
TCG_test_case("invalid column", {
    .input = {
        .board = {
            {5,3,0, 0,7,0, 0,0,0}, /* 5 top left */
            {6,0,0, 1,9,5, 0,0,0},
            {0,9,8, 0,0,0, 0,6,0},

            {8,0,0, 0,6,0, 0,0,3},
            {4,0,0, 8,0,3, 0,0,1},
            {7,0,0, 0,2,0, 0,0,6},

            {0,6,0, 0,0,0, 2,8,0},
            {0,0,0, 4,1,9, 0,0,5},
            {5,0,0, 0,8,0, 0,7,9}, /* 5 bottom left */
        },
    },
    .output = {
        .is_valid = false,
    },
})
TCG_test_case("invalid row", {
    .input = {
        .board = {
            {5,3,0, 0,7,0, 0,0,5}, /* Two 5's in this row. */
            {6,0,0, 1,9,5, 0,0,0},
            {0,9,8, 0,0,0, 0,6,0},

            {8,0,0, 0,6,0, 0,0,3},
            {4,0,0, 8,0,3, 0,0,1},
            {7,0,0, 0,2,0, 0,0,6},

            {0,6,0, 0,0,0, 2,8,0},
            {0,0,0, 4,1,9, 0,0,5},
            {0,0,0, 0,8,0, 0,7,9},
        },
    },
    .output = {
        .is_valid = false,
    },
})
TCG_tests_end(valid_sudoku_tests);

#endif /* VALID_SUDOKU_TESTS_H */
