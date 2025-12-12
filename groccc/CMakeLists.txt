cmake_minimum_required(VERSION 3.23)
project("groccc-problems"
    VERSION 1.0.0
    LANGUAGES C
    DESCRIPTION "Problems to groccc."
)

######################### Fetch C Container Collection ########################

include(FetchContent)
FetchContent_Declare(
  ccc
  URL https://github.com/skeletoss/ccc/releases/download/v0.51.0/ccc-v0.51.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP FALSE
)
FetchContent_MakeAvailable(ccc)
target_compile_options(ccc PRIVATE "-w")

#########################   Fetch Str_view Library     ########################

include(FetchContent)
FetchContent_Declare(
  str_view
  URL https://github.com/skeletoss/str_view/releases/download/v0.7.0/str_view-v0.7.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP FALSE
)
FetchContent_MakeAvailable(str_view)
target_compile_options(str_view PRIVATE "-w")

#########################   Create Problems Target   ##########################

add_custom_target(problems)

########################     Add Problem Runner     ###########################

add_executable(run_problems run_problems.c)
target_link_libraries(run_problems str_view::str_view)
add_dependencies(problems run_problems)

##################   Add Utilities Available to All Tests   ###################

add_library(loggers INTERFACE loggers.h)

add_library(string_arena string_arena.h string_arena.c)

add_library(allocators allocators.h allocators.c)
target_link_libraries(allocators PRIVATE ccc::ccc)

add_library(hash_helpers hash_helpers.h hash_helpers.c)
target_link_libraries(hash_helpers PRIVATE ccc::ccc str_view::str_view)

#######################   Add Test Case Generator Helper   ####################

add_library(test_case_generator INTERFACE test_case_generator.h)
add_dependencies(problems test_case_generator)

#######################   Macro for Adding Problem Files   ####################

macro(add_problem PROBLEM_NAME)
    add_library(${PROBLEM_NAME}_tests INTERFACE ${PROBLEM_NAME}/${PROBLEM_NAME}_tests.h)
    target_link_libraries(${PROBLEM_NAME}_tests INTERFACE ccc::ccc)
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
