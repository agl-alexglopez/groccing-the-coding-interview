#include <stddef.h>

#include "../test_case_generator.h"

struct Two_sum_input
{
    int target;
    int const *const nums;
    size_t nums_count;
};

struct Two_sum_output
{
    int addends[2];
};

TCG_tests_begin(two_sum_tests, struct Two_sum_input, struct Two_sum_output)

TCG_test_case_begin("empty", {})
TCG_test_case_end({
    .addends = {-1, -1}
})

TCG_test_case_begin("negatives", {
    .target = 15,
    .nums = (int[10]){1, 3, -980, 6, 7, 13, 44, 32, 995, -1,},
    .nums_count = 10,
})
TCG_test_case_end({
    .addends = {8, 2},
})

TCG_test_case_begin("no solution", {
    .target = 2,
    .nums = (int[4]){1, 3, 4, 5},
    .nums_count = 4,
})
TCG_test_case_end({
    .addends = {-1, -1}
})

TCG_tests_end(two_sum_tests);
