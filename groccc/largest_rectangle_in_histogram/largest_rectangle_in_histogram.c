#include <stddef.h>

#define BUFFER_USING_NAMESPACE_CCC
#define TRAITS_USING_NAMESPACE_CCC
#include "ccc/buffer.h"
#include "ccc/traits.h"

#include "utility/allocators.h"
#include "utility/loggers.h"
#include "utility/test_case_generator.h"

#include "largest_rectangle_in_histogram_tests.h"

static struct Largest_rectangle_in_histogram_output
largest_rectangle_in_histogram(
    struct Largest_rectangle_in_histogram_input const *const input,
    Buffer *const index_stack)
{
    struct Largest_rectangle_in_histogram_output res = {};
    int const end = (int)count(&input->heights).count;
    for (int i = 0; i <= end; ++i)
    {
        while (!buffer_is_empty(index_stack))
        {
            int const min_height = *buffer_as(
                &input->heights, int, *buffer_back_as(index_stack, int));
            if (i != end && min_height < *buffer_as(&input->heights, int, i))
            {
                break;
            }
            (void)pop_back(index_stack);
            int const width = buffer_is_empty(index_stack)
                                ? i
                                : i - *buffer_back_as(index_stack, int) - 1;
            int const area = min_height * width;
            if (area > res.largest_area)
            {
                res.largest_area = area;
            }
        }
        (void)push_back(index_stack, &i);
    }
    return res;
}

int
main(void)
{
    int passed = 0;
    Buffer index_stack_scratch_buffer
        = buffer_initialize(NULL, int, stdlib_allocate, NULL, 0);
    TCG_for_each_test_case(largest_rectangle_in_histogram_tests, {
        struct Largest_rectangle_in_histogram_output const output
            = largest_rectangle_in_histogram(
                &TCG_test_case_input(largest_rectangle_in_histogram_tests),
                &index_stack_scratch_buffer);
        struct Largest_rectangle_in_histogram_output const *const correct_output
            = &TCG_test_case_output(largest_rectangle_in_histogram_tests);
        if (output.largest_area != correct_output->largest_area)
        {
            logfail(largest_rectangle_in_histogram_tests);
        }
        else
        {
            ++passed;
        }
        clear(&index_stack_scratch_buffer, NULL);
    });
    (void)clear_and_free(&index_stack_scratch_buffer, NULL);
    return TCG_tests_status(largest_rectangle_in_histogram_tests, passed);
}
