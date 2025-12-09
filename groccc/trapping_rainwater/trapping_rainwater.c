#define BUFFER_USING_NAMESPACE_CCC
#define TRAITS_USING_NAMESPACE_CCC

#include "../loggers.h"
#include "trapping_rainwater_test_cases.h"

#include "ccc/buffer.h"
#include "ccc/traits.h"

static inline int
max_int(int const a, int const b)
{
    return a > b ? a : b;
}

static struct Trapping_rainwater_output
trapping_rainwater(struct Trapping_rainwater_input const *input)
{
    Buffer const heights
        = buffer_initialize(input->heights, int, NULL, NULL,
                            input->heights_count, input->heights_count);
    if (buffer_is_empty(&heights))
    {
        return (struct Trapping_rainwater_output){
            .trapped_water_units = 0,
        };
    }
    int trapped = 0;
    int left_peak = *buffer_front_as(&heights, int);
    int right_peak = *buffer_back_as(&heights, int);
    int const *left = next(&heights, begin(&heights));
    int const *right = reverse_next(&heights, reverse_begin(&heights));
    while (left <= right)
    {
        if (left_peak < right_peak)
        {
            left_peak = max_int(left_peak, *left);
            trapped += (left_peak - *left);
            left = next(&heights, left);
        }
        else
        {
            right_peak = max_int(right_peak, *right);
            trapped += (right_peak - *right);
            right = reverse_next(&heights, right);
        }
    }
    return (struct Trapping_rainwater_output){
        .trapped_water_units = trapped,
    };
}

int
main(void)
{
    int passed = 0;
    TCG_for_each_test_case(trapping_rainwater_tests, {
        /* TODO: Use these to check your output against the correct output. */
        struct Trapping_rainwater_output const output = trapping_rainwater(
            &TCG_test_case_input(trapping_rainwater_tests));
        struct Trapping_rainwater_output const *const correct_output
            = &TCG_test_case_output(trapping_rainwater_tests);
        if (output.trapped_water_units != correct_output->trapped_water_units)
        {
            logfail(trapping_rainwater_tests);
        }
        else
        {
            ++passed;
        }
    });
    logstats(trapping_rainwater_tests, passed);
    return 0;
}
