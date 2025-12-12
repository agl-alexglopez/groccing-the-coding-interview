#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define TRAITS_USING_NAMESPACE_CCC
#define BUFFER_USING_NAMESPACE_CCC
#define FLAT_HASH_MAP_USING_NAMESPACE_CCC
#include "ccc/buffer.h"
#include "ccc/flat_hash_map.h"
#include "ccc/traits.h"

#include "../allocators.h"
#include "../hash_helpers.h"
#include "../loggers.h"
#include "../string_arena.h"
#include "group_anagrams_tests.h"

struct String_int
{
    struct String_offset key;
    int val;
};

static CCC_Order
str_view_int_are_equal(CCC_Key_comparator_context const compare)
{
    struct String_offset const *const key = compare.key_left;
    struct String_int const *const type = compare.type_right;
    return (CCC_Order)SV_compare(
        (SV_Str_view){
            .str = string_arena_at(compare.context, key),
            .len = key->len,
        },
        (SV_Str_view){
            .str = string_arena_at(compare.context, &type->key),
            .len = type->key.len,
        });
}

static uint64_t
hash_string_offset(CCC_Key_context const context)
{
    struct String_offset const *const str = context.key;
    return hash_fnv_1a_str_view_to_u64((CCC_Key_context){
        .key = &(SV_Str_view){
            .str = string_arena_at(context.context, str),
            .len = str->len,
        },
    });
}

static void
destroy_nested_buffers(CCC_Type_context const str_view_buffer)
{
    buffer_clear_and_free(str_view_buffer.type, NULL);
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
    if (count(&a->buffer_of_groups).count != count(&b->buffer_of_groups).count)
    {
        return false;
    }
    size_t const end = count(&a->buffer_of_groups).count;
    for (size_t i = 0; i < end; ++i)
    {
        Buffer const *const a_groups = buffer_at(&a->buffer_of_groups, i);
        Buffer const *const b_groups = buffer_at(&b->buffer_of_groups, i);
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
group_anagrams(struct Group_anagrams_input const *input,
               struct String_arena *const str_arena, Buffer *const groups,
               Flat_hash_map *const anagram_map)
{
    int index = 0;
    for (SV_Str_view const *str = begin(&input->strs); str != end(&input->strs);
         str = next(&input->strs, str))
    {
        Buffer chars = buffer_with_compound_literal(27, (int[27]){});
        *buffer_as(&chars, int, 0) = '#';
        for (char const *c = SV_begin(*str); c != SV_end(*str); c = SV_next(c))
        {
            (*buffer_as(&chars, int, *c - 'a' + 1))++;
        }
        int digits_character_count = 1;
        for (int const *freq = next(&chars, begin(&chars)); freq != end(&chars);
             freq = next(&chars, freq))
        {
            digits_character_count += snprintf(NULL, 0, "%d", *freq);
        }
        struct String_int key_value = {
            .key = string_arena_allocate(str_arena, digits_character_count + 1),
            .val = index,
        };
        if (key_value.key.error)
        {
            logerr("string arena memoy exhaustion, exiting now.\n");
            exit(1);
        }
        *string_arena_at(str_arena, &key_value.key) = '#';
        int string_position = 1;
        for (int const *freq = next(&chars, begin(&chars)); freq != end(&chars);
             freq = next(&chars, freq))
        {
            int const characters_needed_for_frequency = snprintf(
                string_arena_at(str_arena, &key_value.key) + string_position,
                digits_character_count, "%d", *freq);
            digits_character_count -= characters_needed_for_frequency;
            string_position += characters_needed_for_frequency;
        }
        CCC_Entry anagram = try_insert(anagram_map, &key_value);
        struct String_int const *const inserted = unwrap(&anagram);
        if (occupied(&anagram))
        {
            /* Save a little space and the string arena will only store unique
               anagram character arrays. */
            string_arena_pop_str(str_arena, &key_value.key);
            Buffer *const group = buffer_at(groups, inserted->val);
            (void)buffer_push_back(group, str);
        }
        else
        {
            buffer_emplace_back(groups, buffer_from(stdlib_allocate, NULL, 0,
                                                    (SV_Str_view[]){*str}));
            ++index;
        }
    }
    return (struct Group_anagrams_output){*groups};
}

int
main(void)
{
    int passed = 0;
    /* We will allow these data structures to act as scratch buffers between
       tests so we are not constantly allocating in a tight testing loop. Just
       remember to clear (not free) their storage between tests. */
    struct String_arena str_arena = string_arena_create(4096);
    Buffer groups = buffer_initialize(NULL, Buffer, stdlib_allocate, NULL, 0);
    Flat_hash_map anagram_map = CCC_flat_hash_map_initialize(
        NULL, struct String_int, key, hash_string_offset,
        str_view_int_are_equal, stdlib_allocate, &str_arena, 0);
    TCG_for_each_test_case(group_anagrams_tests, {
        struct Group_anagrams_output const output
            = group_anagrams(&TCG_test_case_input(group_anagrams_tests),
                             &str_arena, &groups, &anagram_map);
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
        string_arena_clear(&str_arena);
    });
    clear_and_free(&groups, destroy_nested_buffers);
    clear_and_free(&anagram_map, NULL);
    string_arena_free(&str_arena);
    return TCG_tests_status(group_anagrams_tests, passed);
}
