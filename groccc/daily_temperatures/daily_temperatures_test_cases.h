#ifndef DAILY_TEMPERATURES_H
#define DAILY_TEMPERATURES_H

#include "ccc/buffer.h"

#include "../test_case_generator.h"

struct Daily_temperatures_input
{
    CCC_Buffer temperatures;
    CCC_Buffer days_until_warmer_temperature_result;
};

struct Daily_temperatures_output
{
    CCC_Buffer days_until_warmer_temperature;
};

TCG_tests_begin(daily_temperatures_tests,
                struct Daily_temperatures_input,
                struct Daily_temperatures_output)

TCG_test_case("standard gaps", {
    .input = {
        .temperatures = CCC_buffer_initialize(
            ((int[8]){73, 74, 75, 71, 69, 72, 76, 73}),
            int,
            NULL,
            NULL,
            8,
            8
        ),
        .days_until_warmer_temperature_result = CCC_buffer_initialize(
            (int[8]){},
            int,
            NULL,
            NULL,
            8,
        ),
    },
    .output = {
        .days_until_warmer_temperature = CCC_buffer_initialize(
            ((int[8]){1, 1, 4, 2, 1, 1, 0, 0}),
            int,
            NULL,
            NULL,
            8,
            8
        ),
    },
})

TCG_tests_end(daily_temperatures_tests);

#endif /* DAILY_TEMPERATURES_H */
