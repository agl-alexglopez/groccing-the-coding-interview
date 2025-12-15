cmake_minimum_required(VERSION 3.23)
project("groccc-problems"
    VERSION 1.0.0
    LANGUAGES C
    DESCRIPTION "Problems to groccc."
)

#########################   Create Problems Target   ##########################

add_custom_target(problems)

########################     Add Problem Runner     ###########################

add_executable(run_problems run_problems.c)
target_link_libraries(run_problems str_view::str_view loggers test_case_generator)
add_dependencies(problems run_problems)

#######################   Macro for Adding Problem Files   ####################

macro(add_problem PROBLEM_NAME)
    add_library(${PROBLEM_NAME}_tests INTERFACE ${PROBLEM_NAME}/${PROBLEM_NAME}_tests.h)
    target_link_libraries(${PROBLEM_NAME}_tests INTERFACE ccc::ccc str_view::str_view test_case_generator)
    add_executable(${PROBLEM_NAME} ${PROBLEM_NAME}/${PROBLEM_NAME}.c)
    target_link_libraries(${PROBLEM_NAME}
        PRIVATE
            ccc::ccc
            str_view::str_view
            string_arena
            test_case_generator
            loggers
            allocators
            hash_helpers
            ${PROBLEM_NAME}_tests
    )
    set_target_properties(${PROBLEM_NAME}
        PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY
                ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/problems
    )
    add_dependencies(problems ${PROBLEM_NAME})
endmacro()

###########################        Problem List      ##########################

# WARNING! Do not add any comments or text below this comment. A provided script
# appends new coding problem targets to the bottom of this file by creating the 
# necessary files and folders and using the below macro. The script should check
# if problems are unique so this list should not have repeats.

add_problem(two_sum)
add_problem(trapping_rainwater)
add_problem(daily_temperatures)
add_problem(longest_consecutive_sequence)
add_problem(group_anagrams)
add_problem(largest_rectangle_in_histogram)
add_problem(top_k_frequent_elements)
add_problem(valid_sudoku)
