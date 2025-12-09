#include <stddef.h>

#define BUFFER_USING_NAMESPACE_CCC
#define TRAITS_USING_NAMESPACE_CCC

#include "../allocators.h"
#include "../loggers.h"
#include "daily_temperatures_test_cases.h"

#include "ccc/buffer.h"
#include "ccc/traits.h"

static bool
are_equal(Buffer const *const a, Buffer const *const b)
{
    if (count(a).count != count(b).count)
    {
        return false;
    }
    for (int const *a_i = begin(a), *b_i = begin(b);
         a_i != end(a) && b_i != end(b); a_i = next(a, a_i), b_i = next(b, b_i))
    {
        if (*a_i != *b_i)
        {
            return false;
        }
    }
    return true;
}

static struct Daily_temperatures_output
daily_temperatures(struct Daily_temperatures_input *const input)
{
    size_t const end = count(&input->temperatures).count;
    buffer_size_set(&input->days_until_warmer_temperature_result, end);
    Buffer index_stack = buffer_with_capacity(int, stdlib_allocate, NULL, end);
    for (size_t i = 0; i < end; ++i)
    {
        int const *const cur_temp = buffer_at(&input->temperatures, i);
        while (!is_empty(&index_stack)
               && *cur_temp > *buffer_as(&input->temperatures, int,
                                         *buffer_back_as(&index_stack, int)))
        {
            int const index = *buffer_back_as(&index_stack, int);
            *buffer_as(&input->days_until_warmer_temperature_result, int, index)
                = (int)i - index;
            (void)pop_back(&index_stack);
        }
        (void)push_back(&index_stack, &i);
    }
    clear_and_free(&index_stack, NULL);
    return (struct Daily_temperatures_output){
        input->days_until_warmer_temperature_result,
    };
}

int
main(void)
{
    int passed = 0;
    TCG_for_each_test_case(daily_temperatures_tests, {
        struct Daily_temperatures_output const output = daily_temperatures(
            &TCG_test_case_input(daily_temperatures_tests));
        struct Daily_temperatures_output const *const correct_output
            = &TCG_test_case_output(daily_temperatures_tests);
        if (!are_equal(&output.days_until_warmer_temperature,
                       &correct_output->days_until_warmer_temperature))
        {
            logfail(daily_temperatures_tests);
        }
        else
        {
            ++passed;
        }
    });
    logstats(daily_temperatures_tests, passed);
    return 0;
}
