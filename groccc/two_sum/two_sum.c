/** @brief Two Sum
@file
Given an array of integers nums and an integer target, return indices of the two
numbers such that they add up to target.

You may assume that each input would have exactly one solution, and you may not
use the same element twice.

You can return the answer in any order. */
#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#define BUFFER_USING_NAMESPACE_CCC
#define FLAT_HASH_MAP_USING_NAMESPACE_CCC
#define TRAITS_USING_NAMESPACE_CCC
#include "ccc/buffer.h"
#include "ccc/flat_hash_map.h"
#include "ccc/traits.h"
#include "ccc/types.h"

#include "../allocators.h"
#include "../hash_helpers.h"
#include "../loggers.h"
#include "../test_case_generator.h"
#include "two_sum_tests.h"

struct Two_sum_output
two_sum(struct Two_sum_input const *const test_case, Flat_hash_map *const map)
{
    assert(is_empty(map));
    for (int const *i = begin(&test_case->nums); i != end(&test_case->nums);
         i = next(&test_case->nums, i))
    {
        size_t const index = buffer_index(&test_case->nums, i).count;
        struct Int_key_val const *const other_addend
            = get_key_value(map, &(int){
                                     test_case->target - *i,
                                 });
        if (other_addend)
        {
            return (struct Two_sum_output){{
                index,
                other_addend->val,
            }};
        }
        (void)insert_or_assign(
            map, &(struct Int_key_val){
                     .key = *i,
                     .val = buffer_index(&test_case->nums, i).count,
                 });
    }
    return (struct Two_sum_output){};
}

int
main(void)
{
    int passed = 0;
    /* We don't assume any map capacity but will have a working underlying
       buffer that we simply clear between test cases. */
    Flat_hash_map map = flat_hash_map_initialize(
        NULL, struct Int_key_val, key, hash_map_int_to_u64,
        hash_map_int_key_val_order, stdlib_allocate, NULL, 0);
    TCG_for_each_test_case(two_sum_tests, {
        struct Two_sum_output const solution_output
            = two_sum(&TCG_test_case_input(two_sum_tests), &map);
        struct Two_sum_output const *const correct_output
            = &TCG_test_case_output(two_sum_tests);
        if ((solution_output.addends[0] != correct_output->addends[0]
             && solution_output.addends[0] != correct_output->addends[1])
            || (solution_output.addends[1] != correct_output->addends[0]
                && solution_output.addends[1] != correct_output->addends[1]))
        {
            logfail(two_sum_tests);
        }
        else
        {
            ++passed;
        }
        clear(&map, NULL);
    });
    clear_and_free(&map, NULL);
    return TCG_tests_status(two_sum_tests, passed);
}
