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
TCG_test_case("all identical", {
    .input = {
        .strs = CCC_buffer_with_compound_literal(
            5,
            (SV_Str_view[5]){
                SV_from("aa"),
                SV_from("aa"),
                SV_from("aa"),
                SV_from("aa"),
                SV_from("aa"),
            }
        ),
    },
    .output = {
        .groups = CCC_buffer_with_compound_literal(
            1,
            (CCC_Buffer[1]){
                CCC_buffer_with_compound_literal(
                    5,
                    (SV_Str_view[5]){
                        SV_from("aa"),
                        SV_from("aa"),
                        SV_from("aa"),
                        SV_from("aa"),
                        SV_from("aa"),
                    }
                ),
            }
        ),
    },
})
TCG_test_case("no anagrams", {
    .input = {
        .strs = CCC_buffer_with_compound_literal(
            3,
            (SV_Str_view[3]){
                SV_from("abc"),
                SV_from("def"),
                SV_from("ghi"),
            }
        ),
    },
    .output = {
        .groups = CCC_buffer_with_compound_literal(
            3,
            (CCC_Buffer[3]){
                CCC_buffer_with_compound_literal(
                    1, (SV_Str_view[1]){ SV_from("abc") }
                ),
                CCC_buffer_with_compound_literal(
                    1, (SV_Str_view[1]){ SV_from("def") }
                ),
                CCC_buffer_with_compound_literal(
                    1, (SV_Str_view[1]){ SV_from("ghi") }
                ),
            }
        )
    },
})
TCG_test_case("empty input", {
    .input = {
        .strs = CCC_buffer_with_compound_literal(
            0,
            (SV_Str_view[0]){}
        ),
    },
    .output = {
        .groups = CCC_buffer_with_compound_literal(
            0,
            (CCC_Buffer[0]){}
        ),
    },
})
TCG_test_case("single string", {
    .input = {
        .strs = CCC_buffer_with_compound_literal(
            1,
            (SV_Str_view[1]){
                SV_from("solo"),
            }
        ),
    },
    .output = {
        .groups = CCC_buffer_with_compound_literal(
            1,
            (CCC_Buffer[1]){
                CCC_buffer_with_compound_literal(
                    1,
                    (SV_Str_view[1]){ SV_from("solo") }
                ),
            }
        ),
    },
})
TCG_test_case("mixed length anagrams", {
    .input = {
        .strs = CCC_buffer_with_compound_literal(
            7,
            (SV_Str_view[7]){
                SV_from("abcd"),
                SV_from("bcad"),
                SV_from("dabc"),
                SV_from("abc"),
                SV_from("cab"),
                SV_from("bac"),
                SV_from("zzz")
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
                        SV_from("abcd"),
                        SV_from("bcad"),
                        SV_from("dabc"),
                    }
                ),
                CCC_buffer_with_compound_literal(
                    3,
                    (SV_Str_view[3]){
                        SV_from("abc"),
                        SV_from("bac"),
                        SV_from("cab"),
                    }
                ),
                CCC_buffer_with_compound_literal(
                    1,
                    (SV_Str_view[1]){
                        SV_from("zzz"),
                    }
                ),
            }
        ),
    },
})
TCG_tests_end(group_anagrams_tests);

#endif /* GROUP_ANAGRAMS_TESTS_H */
