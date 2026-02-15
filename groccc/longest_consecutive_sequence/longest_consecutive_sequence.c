#include "longest_consecutive_sequence_tests.h"

#define FLAT_HASH_MAP_USING_NAMESPACE_CCC
#define TRAITS_USING_NAMESPACE_CCC
#include "ccc/flat_hash_map.h"
#include "ccc/traits.h"
#include "ccc/types.h"

#include "utility/allocators.h"
#include "utility/hash_helpers.h"
#include "utility/loggers.h"
#include "utility/test_case_generator.h"

static struct Longest_consecutive_sequence_output
longest_consecutive_sequence(struct Longest_consecutive_sequence_input *input,
                             Flat_hash_map *const map)
{
    struct Longest_consecutive_sequence_output result = {};
    for (int const *i = begin(&input->nums); i != end(&input->nums);
         i = next(&input->nums, i))
    {
        CCC_Entry entry
            = try_insert(map, &(struct Int_key_val){.key = *i, .val = 1});
        if (occupied(&entry))
        {
            continue;
        }
        struct Int_key_val *const connect = unwrap(&entry);
        struct Int_key_val const *const left_run
            = get_key_value(map, &(int){*i - 1});
        struct Int_key_val const *const right_run
            = get_key_value(map, &(int){*i + 1});
        int const left_run_length = left_run ? left_run->val : 0;
        int const right_run_length = right_run ? right_run->val : 0;
        int const full_run = left_run_length + connect->val + right_run_length;
        connect->val = full_run;
        if (left_run)
        {
            (void)insert_or_assign(map, &(struct Int_key_val){
                                            .key = *i - left_run_length,
                                            .val = full_run,
                                        });
        }
        if (right_run)
        {
            (void)insert_or_assign(map, &(struct Int_key_val){
                                            .key = *i + right_run_length,
                                            .val = full_run,
                                        });
        }
        if (full_run > result.longest)
        {
            result.longest = full_run;
        }
    }
    return result;
}

int
main(void)
{
    TCG_Count passed = 0;
    /* Not sure how large the test cases are so we will have a scratch map
       we clear on every iteration, leaving its underlying buffer in place
       as it grows to support the maximum array size seen so far. */
    Flat_hash_map map = CCC_flat_hash_map_with_capacity(
        struct Int_key_val, key, hash_map_int_to_u64,
        hash_map_int_key_val_order, stdlib_allocate, 0);
    TCG_for_each_test_case(longest_consecutive_sequence_tests, {
        struct Longest_consecutive_sequence_output const output
            = longest_consecutive_sequence(
                &TCG_test_case_input(longest_consecutive_sequence_tests), &map);
        struct Longest_consecutive_sequence_output const *const correct_output
            = &TCG_test_case_output(longest_consecutive_sequence_tests);
        if (output.longest != correct_output->longest)
        {
            logfail(longest_consecutive_sequence_tests);
        }
        else
        {
            ++passed;
        }
        clear(&map, NULL);
    });
    clear_and_free(&map, NULL);
    return TCG_tests_status(longest_consecutive_sequence_tests, passed);
}
