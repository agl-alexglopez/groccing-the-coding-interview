#ifndef TRAPPING_RAINWATER_TESTS_H
#define TRAPPING_RAINWATER_TESTS_H

#include "ccc/buffer.h"

#include "../test_case_generator.h"

struct Trapping_rainwater_input
{
    CCC_Buffer heights;
};

struct Trapping_rainwater_output
{
    int trapped_water_units;
};

TCG_tests_begin(trapping_rainwater_tests, struct Trapping_rainwater_input,
                struct Trapping_rainwater_output)
TCG_test_case("empty", {.input = {}, .output = {}})
TCG_test_case("trap peak", {
    .input = {
        .heights = CCC_buffer_with_compound_literal(
            3,
            (int[3]){0, 9, 0}
        ),
    },
    .output = {
        .trapped_water_units = 0,
    }
})
TCG_test_case("trap valley", {
    .input = {
        .heights = CCC_buffer_with_compound_literal(
            3,
            (int[3]){9, 0, 9}
        ),
    },
    .output = {
        .trapped_water_units = 9,
    }
})
TCG_test_case("trap 6", {
    .input = {
        .heights = CCC_buffer_with_compound_literal(
            12,
            (int[12]){0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1}
        ),
    },
    .output = {
        .trapped_water_units = 6,
    },
})
TCG_test_case("trap 10", {
    .input = {
        .heights = CCC_buffer_with_compound_literal(
            8,
            (int[8]){4, 2, 0, 3, 2, 5, 1, 2}
        ),
    },
    .output = {
        .trapped_water_units = 10,
    }
})
TCG_test_case("trap pockets on way to peak", {
    .input = {
        .heights = CCC_buffer_with_compound_literal(
            7,
            (int[7]){2, 1, 4, 7, 4, 1, 2}
        ),
    },
    .output = {
        .trapped_water_units = 2,
    }
})
TCG_test_case("trap pockets on way to valley", {
    .input = {
        .heights = CCC_buffer_with_compound_literal(
            7,
            (int[7]){7, 1, 4, 3, 2, 1, 0}
        ),
    },
    .output = {
        .trapped_water_units = 3,
    }
})
TCG_tests_end(trapping_rainwater_tests);

#endif /* TRAPPING_RAINWATER_TESTS_H */
