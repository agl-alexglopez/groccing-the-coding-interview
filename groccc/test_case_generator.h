/** @file
@brief The Test Case Generator Interface

The test case generator allows for easier generation and running of tests while
maximizing programmer freedom in structuring solutions. This interface does not
introduce any dependencies, allowing user type definitions and code blocks to
define the entirety of a problem solution and test case interaction. This
interface simply eliminates all boilerplate. Here is a full usage example for
the two sum problem.

Define test cases in a separate header. Every test case is given a `name`,
`input`, and `output` field according to the user provided types.

```
#ifndef TWO_SUM_TEST_CASES_H
#define TWO_SUM_TEST_CASES_H
#include <stddef.h>

#include "../test_case_generator.h"

struct Two_sum_input
{
    int target;
    int const *const nums;
    size_t nums_count;
};

struct Two_sum_output
{
    int addends[2];
};

TCG_tests_begin(two_sum_tests, struct Two_sum_input, struct Two_sum_output)
TCG_test_case("empty", {
    .input = {},
    .output = {
        .addends = {-1, -1}
    },
})
TCG_test_case("negatives", {
    .input = {
        .target = 15,
        .nums = (int[10]){1, 3, -980, 6, 7, 13, 44, 32, 995, -1,},
        .nums_count = 10,
    },
    .output = {
        .addends = {8, 2},
    }
})
TCG_test_case("no solution", {
    .input = {
        .target = 2,
        .nums = (int[4]){1, 3, 4, 5},
        .nums_count = 4,
    },
    .output = {
        .addends = {-1, -1}
    },
})
TCG_tests_end(two_sum_tests);
#endif // TWO_SUM_TEST_CASES_H
```

Then the calling code utilizes the types the user defined for test cases and the
generator machinery code as follows.

```
struct Int_key_val
{
    int key;
    int val;
};

struct Two_sum_output
two_sum(struct Two_sum_input const *const test_case)
{
    Flat_hash_map map = flat_hash_map_initialize(
        &(Small_fixed_map){}, struct Int_key_val, key, flat_hash_map_int_to_u64,
        flat_hash_map_id_order, NULL, NULL,
        flat_hash_map_fixed_capacity(Small_fixed_map));
    struct Two_sum_output solution = {{-1, -1}};
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
    int passed = 0;
    TCG_for_each_test_case(two_sum_tests, {
        struct Two_sum_output const solution_output
            = two_sum(&TCG_test_case_input(two_sum_tests));
        struct Two_sum_output const *const correct_output
            = &TCG_test_case_output(two_sum_tests);
        if (!solution_matches_output(&solution_output, correct_output))
        {
            (void)fprintf(stderr, "fail for test: %s, file: %s, line: %d\n",
                          TCG_test_case_name(two_sum_tests),
                          TCG_test_case_file(two_sum_tests),
                          TCG_test_case_line(two_sum_tests));
        }
        else
        {
            ++passed;
        }
    });
    (void)fprintf(stdout, "two_sum passed %d/%lu\n", passed,
                  TCG_tests_count(two_sum_tests));
    return 0;
}
```

Notice that the user writes the code block that handles running the solution
function, validating the solution, and handling error reporting. This allows
every problem file to be well tailored to the needed types. Also, test input
and output can be obtained by value with the provided macros and by reference
by simply adding the address of operator, `&`, as in the provided example.

Finally, note that the test case struct and input field are mutable while the
output field is declared const. This is done so that certain problem solutions
that may wish to modify input as part of the solution may do so. The output
should never be mutated. */
#ifndef TCG_TEST_CASE_GENERATOR_H
#define TCG_TEST_CASE_GENERATOR_H

enum TCG_Tests_status : char /* NOLINT */
{
    TCG_TESTS_ERROR = -1,
    TCG_TESTS_PASS = 0,
    TCG_TESTS_FAIL = 1,
};

/** @brief Create a custom test case type struct with input and output types.
@param[in] test_cases_name the name of the struct holding all test cases.
@param[in] input_type the struct name of the input type. For clarity, define
structs with named fields for all input, even if a single field.
@param[in] output_type the struct name of the expected output type. For clarity,
define structs with named fields for all output, even if a single field.
@warning a terminating semicolon is not needed when calling this macro.
@warning output is a constant field and cannot be mutated. */
#define TCG_tests_begin(test_cases_name, input_type, output_type)              \
    static struct                                                              \
    {                                                                          \
        char const *const file;                                                \
        int line;                                                              \
        char const *const name;                                                \
        struct                                                                 \
        {                                                                      \
            input_type input;                                                  \
            output_type const output;                                          \
        };                                                                     \
    }(test_cases_name)[] = {

/** @brief Create a test case within a test struct by name, input, and output
@param[in] test_name the string literal name used to describe the test.
@param brace_enclosed_input_output_initializers the field designated
initializers for the input and expected output of a test case.
@warning Always enclose the second argument in `{}` braces and label the
`.input` and `.output` fields.

Here is a minimal usage example

```
struct Two_sum_input
{
    int target;
    int const *const nums;
    size_t nums_count;
};
struct Two_sum_output
{
    int addends[2];
};
TCG_tests_begin(two_sum_tests, struct Two_sum_input, struct Two_sum_output)
TCG_test_case("negatives", {
    .input = {
        .target = 15,
        .nums = (int[10]){1, 3, -980, 6, 7, 13, 44, 32, 995, -1,},
        .nums_count = 10,
    },
    .output = {
        .addends = {8, 2},
    }
})
TCG_tests_end(two_sum_tests);
```

The names of the input and output fields are always the same but their types
are defined for each test by the user. */
#define TCG_test_case(test_name, brace_enclosed_input_output_initializers...)  \
    {                                                                          \
        .file = __FILE_NAME__,                                                 \
        .line = __LINE__,                                                      \
        .name = test_name,                                                     \
        brace_enclosed_input_output_initializers,                              \
    },

/** @brief End the test cases defined for the given struct.
@param[in] test_cases_name the same name used for the begin macro.
@warning a terminating semicolon is needed when calling this macro. */
#define TCG_tests_end(test_cases_name)                                         \
    }                                                                          \
    ;                                                                          \
    static const unsigned long tcg_count_##test_cases_name                     \
        = sizeof(test_cases_name) / sizeof((test_cases_name)[0])

/** @brief Retrieve the unsigned long long count of tests for this test case
struct.
@param[in] test_cases_name the name of the current test cases struct.
@return the unsigned count of tests contained within this struct.

This can be helpful if the user is tracking how many tests are being passed or
failed while iterating over the test cases struct. */
#define TCG_tests_count(test_cases_name) tcg_count_##test_cases_name

/** @brief Obtain the string name given to a test.
@param[in] test_cases_name the name of the current test cases struct being ran.
@return the string name given by the user to this test case.

Providing informative names to each test can help understand their purpose. */
#define TCG_test_case_name(test_cases_name) test_cases_name[tcg_index].name

/** @brief Retrieves the string name of the file from which the test came.
@param[in] test_cases_name the name of the current test cases struct being ran.
@return the string name of the file from which the test case struct can be
found.

Obtaining the file in combination with the line number of the current test can
make tracking down a failing test easier. */
#define TCG_test_case_file(test_cases_name) test_cases_name[tcg_index].file

/** @brief Retrieves the line number of the current test.
@param[in] test_cases_name the name of the current test cases struct.
@return the line number of the current test being ran from its source file. The
line number will correspond to the beginning of the test case provided where
the user wrote the begin test case macro.

Obtaining the line number in combination with the file name can make tracking
down a failing test easier. */
#define TCG_test_case_line(test_cases_name) test_cases_name[tcg_index].line

/** @brief Retrieves the input object of the current test being
processed in the for each test case macro.
@param[in] test_cases_name the name of the test cases being iterated over.
@return the current test case input struct type.
@note if a reference to this input instance is desired simply take the address
of the return value, as one would in normal array-wise iteration.

```
struct My_input_type const *input = &TCG_test_case_input(my_test_cases);
```

The user can decide if retrieve by copy or reference is preferred. */
#define TCG_test_case_input(test_cases_name) test_cases_name[tcg_index].input

/** @brief Retrieves the output object of the current test being
processed in the for each test case macro.
@param[in] test_cases_name the name of the test cases being iterated over.
@return the current test case output struct type.
@note if a reference to this output instance is desired simply take the address
of the return value, as one would in normal array-wise iteration.

```
struct My_output_type const *output = &TCG_test_case_output(my_test_cases);
```

The user can decide if retrieve by copy or reference is preferred. */
#define TCG_test_case_output(test_cases_name) test_cases_name[tcg_index].output

/** @brief Runs the provided solution, comparison code, and cleanup code over
the user generated test code.
@param[in] test_cases_name the static constant test cases structure.
@param solution_code_comparison_code_cleanup_code the code needed to run the
solution function with the test case input type as input, compare the function
output to the test case output, and clean up any allocations as needed.
@note Use the provided name, input, and output getter macros to aid in
processing each test case.

This function lets the user problem executable determine what comparison and
test failure looks like. It does not bring in any print or assert dependencies.
Comparing scalars, arrays, and allocations can be tricky so the user can write
the most type correct code in the provided code block. */
#define TCG_for_each_test_case(test_cases_name,                                \
                               solution_code_comparison_code_cleanup_code...)  \
    (__extension__({                                                           \
        for (unsigned long tcg_index = 0;                                      \
             tcg_index < TCG_tests_count(test_cases_name); ++tcg_index)        \
        {                                                                      \
            solution_code_comparison_code_cleanup_code                         \
        }                                                                      \
    }))

/** @brief A POSIX compliant return value status for a problem executable.
@param[in] test_cases_name the test cases struct being solved.
@param[in] passed_count the user maintained passed count based on user
implemented correctness checks of input and output.
@return a passing status of 0 if all tests pass or 1 if any tests did not pass.
@note if an error status needs to be returned, do so separately. An error is
not one of the two possible values for this macro.

This is intended to formalize the return value of all problem executables in
the case that they are run from a parent process. This allows for meaningful
interprocess communication of the result of running test cases for that problem.
The return values are POSIX compliant, choosing 0 for a good status and 1 for
any errors or failed tests encountered. The -1 status may be used if an
unexpected error occurs and can be returned to the parent process. However,
that is not part of the two possible returns here. */
#define TCG_tests_status(test_cases_name, passed_count)                        \
    ((typeof(TCG_tests_count(test_cases_name)))(passed_count)                  \
     == TCG_tests_count(test_cases_name))                                      \
        ? TCG_TESTS_PASS                                                       \
        : TCG_TESTS_FAIL

#endif /* TCG_TEST_CASE_GENERATOR_H */
