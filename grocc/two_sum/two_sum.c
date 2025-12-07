#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define FLAT_HASH_MAP_USING_NAMESPACE_CCC
#define TRAITS_USING_NAMESPACE_CCC
#include "ccc/flat_hash_map.h"
#include "ccc/traits.h"
#include "ccc/types.h"

#include "../test_case_generator.h"
#include "two_sum_test_cases.h"

struct Int_key_val
{
    int key;
    int val;
};

/** A small fixed map is good when 64 is a desirable upper bound on capacity.
Insertions can continue for about 87.5% of the capacity so about 56. Play it
safe and avoid this limit unless testing insertion failure is important. */
flat_hash_map_declare_fixed_map(Small_fixed_map, struct Int_key_val, 64);

static CCC_Order
flat_hash_map_id_order(CCC_Key_comparator_context const order)
{
    struct Int_key_val const *const right = order.type_right;
    int const left = *((int *)order.key_left);
    return (left > right->key) - (left < right->key);
}

static uint64_t
flat_hash_map_int_to_u64(CCC_Key_context const k)
{
    int const id_int = *((int *)k.key);
    uint64_t x = id_int;
    x = (x ^ (x >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
    x = (x ^ (x >> 27)) * UINT64_C(0x94d049bb133111eb);
    x = x ^ (x >> 31);
    return x;
}

struct Two_sum_output
two_sum(struct Two_sum_input const *const test_case)
{
    Flat_hash_map map = flat_hash_map_initialize(
        &(Small_fixed_map){}, struct Int_key_val, key, flat_hash_map_int_to_u64,
        flat_hash_map_id_order, NULL, NULL,
        flat_hash_map_fixed_capacity(Small_fixed_map));
    struct Two_sum_output solution = {
        .addends = {-1, -1},
    };
    for (size_t i = 0; i < test_case->nums_count; ++i)
    {
        struct Int_key_val const *const other_addend = get_key_value(
            &map, &(int){test_case->target - test_case->nums[i]});
        if (other_addend)
        {
            solution.addends[0] = (int)i;
            solution.addends[1] = other_addend->val;
        }
        (void)insert_or_assign(&map, &(struct Int_key_val){
                                         .key = test_case->nums[i],
                                         .val = (int)i,
                                     });
    }
    return solution;
}

int
main(void)
{
    TCG_for_each_test_case(two_sum_tests, {
        struct Two_sum_output const solution_output
            = two_sum(&TCG_test_case_input(two_sum_tests));
        struct Two_sum_output const *const correct_output
            = &TCG_test_case_output(two_sum_tests);
        if ((solution_output.addends[0] != correct_output->addends[0]
             && solution_output.addends[0] != correct_output->addends[1])
            || (solution_output.addends[1] != correct_output->addends[0]
                && solution_output.addends[1] != correct_output->addends[1]))
        {
            (void)fprintf(stderr, "fail for test: %s, file: %s, line: %d\n",
                          TCG_test_case_name(two_sum_tests),
                          TCG_test_case_file(two_sum_tests),
                          TCG_test_case_line(two_sum_tests));
            return 1;
        }
    });
    return 0;
}
