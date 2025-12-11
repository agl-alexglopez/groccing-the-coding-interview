#ifndef GROUP_ANAGRAMS_H
#define GROUP_ANAGRAMS_H

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
        .strs = CCC_buffer_initialize(
            ((SV_Str_view[6]){
                SV_from("eat"),
                SV_from("tea"),
                SV_from("tan"),
                SV_from("ate"),
                SV_from("nat"),
                SV_from("bat"),
            }), SV_Str_view, NULL, NULL, 6, 6
        ),
    },
    .output = {
        .groups = CCC_buffer_initialize(
            ((CCC_Buffer[3]){
                CCC_buffer_initialize(
                    ((SV_Str_view[3]){
                        SV_from("ate"),
                        SV_from("eat"),
                        SV_from("tea")
                     }), SV_Str_view, NULL, NULL, 3, 3
                ),
                CCC_buffer_initialize(
                    ((SV_Str_view[2]){
                        SV_from("nat"),
                        SV_from("tan")
                     }), SV_Str_view, NULL, NULL, 2, 2
                ),
                CCC_buffer_initialize(
                    ((SV_Str_view[1]){
                        SV_from("bat")
                     }), SV_Str_view, NULL, NULL, 1, 1
                ),
            }), CCC_Buffer, NULL, NULL, 3, 3
        )
    },
})
TCG_tests_end(group_anagrams_tests);

#endif /* GROUP_ANAGRAMS_H */
