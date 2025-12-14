#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#define BUFFER_USING_NAMESPACE_CCC
#define TRAITS_USING_NAMESPACE_CCC
#define FLAT_HASH_MAP_USING_NAMESPACE_CCC
#define FLAT_PRIORITY_QUEUE_USING_NAMESPACE_CCC
#include "ccc/buffer.h"
#include "ccc/flat_hash_map.h"
#include "ccc/flat_priority_queue.h"
#include "ccc/traits.h"
#include "ccc/types.h"

#include "utility/allocators.h"
#include "utility/hash_helpers.h"
#include "utility/loggers.h"
#include "utility/test_case_generator.h"

#include "top_k_frequent_elements_tests.h"

static inline bool
are_equal(Buffer const *const a, Buffer const *const b)
{
    if (count(a).count != count(b).count)
    {
        return false;
    }
    if (!count(a).count)
    {
        return true;
    }
    return memcmp(begin(a), begin(b), buffer_count_bytes(a).count) == 0;
}

static CCC_Order
compare_heap_elements(CCC_Type_comparator_context const context)
{
    Flat_hash_map const *const frequency = context.context;
    int const *const lhs = context.type_left;
    int const *const rhs = context.type_right;
    struct Int_key_val const *const lhs_frequency
        = get_key_value(frequency, lhs);
    struct Int_key_val const *const rhs_frequency
        = get_key_value(frequency, rhs);
    assert(lhs_frequency);
    assert(rhs_frequency);
    return (lhs_frequency->val > rhs_frequency->val)
         - (lhs_frequency->val < rhs_frequency->val);
}

/** I like the heap sort method because it is fast, uses very little space,
and tests more of the Collection data structures. Bucket sort technically meets
the runtime specification but so what. */
static struct Top_k_frequent_elements_output
top_k_frequent_elements(struct Top_k_frequent_elements_input const *const input,
                        Buffer *const top_k, Flat_hash_map *const frequency)
{
    for (int const *i = begin(&input->nums); i != end(&input->nums);
         i = next(&input->nums, i))
    {
        flat_hash_map_or_insert_with(
            (flat_hash_map_and_modify_with(entry_wrap(frequency, i),
                                           struct Int_key_val, { ++T->val; })),
            (struct Int_key_val){
                .key = *i,
                .val = 1,
            });
    }
    if (count(frequency).count < (size_t)input->k)
    {
        return (struct Top_k_frequent_elements_output){};
    }
    Buffer heap_storage = buffer_with_capacity(int, stdlib_allocate, NULL,
                                               count(frequency).count);
    for (struct Int_key_val const *i = begin(frequency); i != end(frequency);
         i = next(frequency, i))
    {
        (void)push_back(&heap_storage, i);
    }
    /* The priority queue does not need allocation permissions. It just wraps
       the provided buffer and orders it. */
    Flat_priority_queue max_heap = flat_priority_queue_heapify_initialize(
        begin(&heap_storage), int, CCC_ORDER_GREATER, compare_heap_elements,
        NULL, frequency, capacity(&heap_storage).count,
        count(&heap_storage).count);
    int to_push = input->k;
    while (to_push && !is_empty(&max_heap))
    {
        (void)push_back(top_k, front(&max_heap));
        pop(&max_heap, &(int){0});
        --to_push;
    }
    clear_and_free(&heap_storage, NULL);
    return (struct Top_k_frequent_elements_output){*top_k};
}

int
main(void)
{
    int passed = 0;
    Flat_hash_map frequency_scratch_map = flat_hash_map_initialize(
        NULL, struct Int_key_val, key, hash_map_int_to_u64,
        hash_map_int_key_val_order, stdlib_allocate, NULL, 0);
    Buffer top_k_scratch_buffer
        = buffer_initialize(NULL, int, stdlib_allocate, NULL, 0);
    TCG_for_each_test_case(top_k_frequent_elements_tests, {
        struct Top_k_frequent_elements_output const output
            = top_k_frequent_elements(
                &TCG_test_case_input(top_k_frequent_elements_tests),
                &top_k_scratch_buffer, &frequency_scratch_map);
        struct Top_k_frequent_elements_output const *const correct_output
            = &TCG_test_case_output(top_k_frequent_elements_tests);
        if (!are_equal(&output.top_k, &correct_output->top_k))
        {
            logfail(top_k_frequent_elements_tests);
        }
        else
        {
            ++passed;
        }
        clear(&top_k_scratch_buffer, NULL);
        clear(&frequency_scratch_map, NULL);
    });
    clear_and_free(&top_k_scratch_buffer, NULL);
    clear_and_free(&frequency_scratch_map, NULL);
    return TCG_tests_status(top_k_frequent_elements_tests, passed);
}
