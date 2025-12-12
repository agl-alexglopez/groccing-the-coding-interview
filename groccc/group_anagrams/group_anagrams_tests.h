#ifndef GROUP_ANAGRAMS_TESTS_H
#define GROUP_ANAGRAMS_TESTS_H

#include "../test_case_generator.h"

#include "ccc/buffer.h"
#include "str_view/str_view.h"

/** The input strings to the problem. */
struct Group_anagrams_input
{
    /** A buffer of SV_Str_view strings. */
    CCC_Buffer strs;
};

/** The output buffer of buffer anagram SV_Str_view groups. */
struct Group_anagrams_output
{
    /** A buffer of buffers. Each buffer groups the string view anagrams. */
    CCC_Buffer groups;
};

TCG_tests_begin(group_anagrams_tests,
                struct Group_anagrams_input,
                struct Group_anagrams_output)
TCG_test_case("3 groups", {
    .input = {
        .strs = CCC_buffer_with_compound_literal(
            6,
            (SV_Str_view[6]){
                SV_from("eat"),
                SV_from("tea"),
                SV_from("tan"),
                SV_from("ate"),
                SV_from("nat"),
                SV_from("bat"),
            }
        ),
    },
    .output = {
        .groups = CCC_buffer_with_compound_literal(
            3,
            (CCC_Buffer[3]){
                CCC_buffer_with_compound_literal(
                    3,
                    (SV_Str_view[3]){
                        SV_from("ate"),
                        SV_from("eat"),
                        SV_from("tea"),
                    }
                ),
                CCC_buffer_with_compound_literal(
                    2,
                    (SV_Str_view[2]){
                        SV_from("nat"),
                        SV_from("tan"),
                    }
                ),
                CCC_buffer_with_compound_literal(
                    1,
                    (SV_Str_view[1]){
                        SV_from("bat"),
                    }
                ),
            }
        )
    },
})
TCG_tests_end(group_anagrams_tests);

#endif /* GROUP_ANAGRAMS_TESTS_H */
