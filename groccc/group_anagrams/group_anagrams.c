#include <stdbool.h>
#include <stddef.h>

#define TRAITS_USING_NAMESPACE_CCC
#define BUFFER_USING_NAMESPACE_CCC
#define FLAT_HASH_MAP_USING_NAMESPACE_CCC
#include "ccc/buffer.h"
#include "ccc/flat_hash_map.h"
#include "ccc/traits.h"

#include "../allocators.h"
#include "../hash_helpers.h"
#include "../loggers.h"
#include "group_anagrams_test_cases.h"

struct Str_view_int
{
    int key[27];
    SV_Str_view key_view;
    int val;
};

static CCC_Order
str_view_int_are_equal(CCC_Key_comparator_context const compare)
{
    SV_Str_view const *const key = compare.key_left;
    struct Str_view_int const *const type = compare.type_right;
    return (CCC_Order)SV_compare(*key, type->key_view);
}

static void
destroy_nested_buffers(CCC_Type_context const str_view_buffer)
{
    Buffer *const buffer = str_view_buffer.type;
    clear_and_free(buffer, NULL);
}

static inline bool
contains_str(Buffer const *const strs, SV_Str_view const *const str)
{
    for (SV_Str_view const *s = begin(strs); s != end(strs); s = next(strs, s))
    {
        if (SV_compare(*s, *str) == SV_ORDER_EQUAL)
        {
            return true;
        }
    }
    return false;
}

static inline bool
is_correct(struct Group_anagrams_output const *const a,
           struct Group_anagrams_output const *const b)
{
    if (count(&a->groups).count != count(&b->groups).count)
    {
        return false;
    }
    size_t const end = count(&a->groups).count;
    for (size_t i = 0; i < end; ++i)
    {
        Buffer const *const a_groups = buffer_at(&a->groups, i);
        Buffer const *const b_groups = buffer_at(&b->groups, i);
        if (count(a_groups).count != count(b_groups).count)
        {
            return false;
        }
        for (SV_Str_view const *str = begin(a_groups); str != end(a_groups);
             str = next(a_groups, str))
        {
            if (!contains_str(b_groups, str))
            {
                return false;
            }
        }
    }
    return true;
}

static struct Group_anagrams_output
group_anagrams(struct Group_anagrams_input const *input, Buffer *const groups,
               Flat_hash_map *const anagram_map)
{
    int index = 0;
    for (SV_Str_view const *str = begin(&input->strs); str != end(&input->strs);
         str = next(&input->strs, str))
    {
        int chars[27] = {};
        chars[0] = '#';
        for (char const *c = SV_begin(*str); c != SV_end(*str); c = SV_next(c))
        {
            ++chars[*c - 'a' + 1];
        }
        struct Str_view_int key_value = {
        .key_view = {
            .str = (char const *)chars,
            .len = sizeof(chars),
        },
        .val = index,
        };
        (void)memcpy(key_value.key, chars, sizeof(chars));
        CCC_Entry anagram = try_insert(anagram_map, &key_value);
        struct Str_view_int const *const inserted = unwrap(&anagram);
        if (occupied(&anagram))
        {
            Buffer *const group = buffer_at(groups, inserted->val);
            (void)buffer_push_back(group, str);
        }
        else
        {
            buffer_emplace_back(groups, buffer_from(stdlib_allocate, NULL, 0,
                                                    (SV_Str_view[]){
                                                        *str,
                                                    }));
            ++index;
        }
    }
    return (struct Group_anagrams_output){
        *groups,
    };
}

int
main(void)
{
    int passed = 0;
    Buffer groups = buffer_initialize(NULL, Buffer, stdlib_allocate, NULL, 0);
    Flat_hash_map anagram_map = CCC_flat_hash_map_initialize(
        NULL, struct Str_view_int, key_view, hash_fnv_1a_str_view_to_u64,
        str_view_int_are_equal, stdlib_allocate, NULL, 0);
    TCG_for_each_test_case(group_anagrams_tests, {
        struct Group_anagrams_output const output = group_anagrams(
            &TCG_test_case_input(group_anagrams_tests), &groups, &anagram_map);
        struct Group_anagrams_output const *const correct_output
            = &TCG_test_case_output(group_anagrams_tests);
        if (!is_correct(&output, correct_output))
        {
            logfail(group_anagrams_tests);
        }
        else
        {
            ++passed;
        }
        clear(&groups, destroy_nested_buffers);
        clear(&anagram_map, NULL);
    });
    clear_and_free(&groups, destroy_nested_buffers);
    clear_and_free(&anagram_map, NULL);
    return TCG_tests_status(group_anagrams_tests, passed);
}
