#ifndef DAILY_TEMPERATURES_TESTS_H
#define DAILY_TEMPERATURES_TESTS_H

#include "ccc/buffer.h"

#include "../test_case_generator.h"

/** The input temperature buffer with convenience output solution buffer
provided. Because the daily temperatures problem specifies the return value
has an entry for each temperature, it is provided here. The result buffer
will be provided with capacity equivalent to temperatures but 0 count.

Note that the result buffer may be freely and trivially shallow copied and
returned from a function. The underlying static memory to which it points
does not move and metadata in the Buffer struct remains consistent. */
struct Daily_temperatures_input
{
    /** The input buffer of N temperatures. */
    CCC_Buffer temperatures;
    /** The return value for this problem of N entries representing the number
    of days until a warmer temperature from the day at that index. Has
    capacity equivalent to temperatures and initial count 0. May be shallow
    copied into output return struct safely. Underlying storage does not
    move. */
    CCC_Buffer days_until_warmer_temperature_result;
};

/** The correct output buffer of N entries corresponding to the temperature for
each day. Each entry at index i represents the number of days required to see
a warmer temperature than the temperature recorded at the input buffer. */
struct Daily_temperatures_output
{
    /** The output buffer of correct days until warmer temperatures. */
    CCC_Buffer days_until_warmer_temperature;
};

TCG_tests_begin(daily_temperatures_tests,
                struct Daily_temperatures_input,
                struct Daily_temperatures_output)
TCG_test_case("empty", {
    .input = {
        .temperatures = CCC_buffer_with_compound_literal(
            0,
            (int[0]){}
        ),
        .days_until_warmer_temperature_result = CCC_buffer_with_compound_literal(
            0,
            (int[0]){}
        ),
    },
    .output = {
        .days_until_warmer_temperature = CCC_buffer_with_compound_literal(
            0,
            (int[0]){}
        ),
    },
})
TCG_test_case("single element", {
    .input = {
        .temperatures = CCC_buffer_with_compound_literal(
            1,
            (int[1]){42}
        ),
        .days_until_warmer_temperature_result = CCC_buffer_with_compound_literal(
            1,
            (int[1]){}
        ),
    },
    .output = {
        .days_until_warmer_temperature = CCC_buffer_with_compound_literal(
            1,
            (int[1]){0}
        ),
    },
})
TCG_test_case("standard gaps", {
    .input = {
        .temperatures = CCC_buffer_with_compound_literal(
            8,
            (int[8]){73, 74, 75, 71, 69, 72, 76, 73}
        ),
        .days_until_warmer_temperature_result = CCC_buffer_with_compound_literal(
            8,
            (int[8]){}
        ),
    },
    .output = {
        .days_until_warmer_temperature = CCC_buffer_with_compound_literal(
            8,
            (int[8]){1, 1, 4, 2, 1, 1, 0, 0}
        ),
    },
})
TCG_test_case("ascending with dips", {
    .input = {
        .temperatures = CCC_buffer_with_compound_literal(
            5,
            (int[5]){30, 60, 90, 40, 50}
        ),
        .days_until_warmer_temperature_result = CCC_buffer_with_compound_literal(
            5,
            (int[5]){}
        ),
    },
    .output = {
        .days_until_warmer_temperature = CCC_buffer_with_compound_literal(
            5,
            (int[5]){1, 1, 0, 1, 0}
        ),
    },
})
TCG_test_case("all equal temperatures", {
    .input = {
        .temperatures = CCC_buffer_with_compound_literal(
            5,
            (int[5]){50, 50, 50, 50, 50}
        ),
        .days_until_warmer_temperature_result = CCC_buffer_with_compound_literal(
            5,
            (int[5]){}
        ),
    },
    .output = {
        .days_until_warmer_temperature = CCC_buffer_with_compound_literal(
            5,
            (int[5]){0, 0, 0, 0, 0}
        ),
    },
})
TCG_test_case("strictly increasing", {
    .input = {
        .temperatures = CCC_buffer_with_compound_literal(
            6,
            (int[6]){10, 20, 30, 40, 50, 60}
        ),
        .days_until_warmer_temperature_result = CCC_buffer_with_compound_literal(
            6,
            (int[6]){}
        ),
    },
    .output = {
        .days_until_warmer_temperature = CCC_buffer_with_compound_literal(
            6,
            (int[6]){1, 1, 1, 1, 1, 0}
        ),
    },
})
TCG_test_case("strictly decreasing", {
    .input = {
        .temperatures = CCC_buffer_with_compound_literal(
            6,
            (int[6]){90, 80, 70, 60, 50, 40}
        ),
        .days_until_warmer_temperature_result = CCC_buffer_with_compound_literal(
            6,
            (int[6]){}
        ),
    },
    .output = {
        .days_until_warmer_temperature = CCC_buffer_with_compound_literal(
            6,
            (int[6]){0, 0, 0, 0, 0, 0}
        ),
    },
})
TCG_test_case("alternating high low", {
    .input = {
        .temperatures = CCC_buffer_with_compound_literal(
            8,
            (int[8]){50, 40, 50, 40, 50, 40, 50, 40}
        ),
        .days_until_warmer_temperature_result = CCC_buffer_with_compound_literal(
            8,
            (int[8]){}
        ),
    },
    .output = {
        .days_until_warmer_temperature = CCC_buffer_with_compound_literal(
            8,
            (int[8]){0, 1, 0, 1, 0, 1, 0, 0}
        ),
    },
})
TCG_test_case("plateaus with rising edges", {
    .input = {
        .temperatures = CCC_buffer_with_compound_literal(
            7,
            (int[7]){30, 30, 30, 40, 40, 50, 50}
        ),
        .days_until_warmer_temperature_result = CCC_buffer_with_compound_literal(
            7,
            (int[7]){}
        ),
    },
    .output = {
        .days_until_warmer_temperature = CCC_buffer_with_compound_literal(
            7,
            (int[7]){3, 2, 1, 2, 1, 0, 0}
        ),
    },
})
TCG_test_case("large valley then spike", {
    .input = {
        .temperatures = CCC_buffer_with_compound_literal(
            7,
            (int[7]){80, 60, 40, 20, 10, 70, 30}
        ),
        .days_until_warmer_temperature_result = CCC_buffer_with_compound_literal(
            7,
            (int[7]){}
        ),
    },
    .output = {
        .days_until_warmer_temperature = CCC_buffer_with_compound_literal(
            7,
            (int[7]){0, 4, 3, 2, 1, 0, 0}
        ),
    },
})
TCG_test_case("jagged mixed pattern", {
    .input = {
        .temperatures = CCC_buffer_with_compound_literal(
            10,
            (int[10]){55, 58, 53, 54, 56, 52, 60, 59, 61, 50}
        ),
        .days_until_warmer_temperature_result = CCC_buffer_with_compound_literal(
            10,
            (int[10]){}
        ),
    },
    .output = {
        .days_until_warmer_temperature = CCC_buffer_with_compound_literal(
            10,
            (int[10]){1, 5, 1, 1, 2, 1, 2, 1, 0, 0}
        ),
    },
})
TCG_tests_end(daily_temperatures_tests);

#endif /* DAILY_TEMPERATURES_TESTS_H */
