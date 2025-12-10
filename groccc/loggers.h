#ifndef LOGGERS_H
#define LOGGERS_H

#include <stdio.h> /* NOLINT */

#include "test_case_generator.h"

#define LOG_RED "\033[38;5;9m"
#define LOG_GREEN "\033[38;5;10m"
#define LOG_CYAN "\033[38;5;14m"
#define LOG_NONE "\033[0m"

#define logout(format_string...) (void)fprintf(stdout, format_string);

#define logerr(format_string...) (void)fprintf(stderr, format_string);

#define logfail(test_cases_name)                                               \
    (void)fprintf(stderr, "\n%sFailed \"%s\" in %s on line %d.%s\n", LOG_RED,  \
                  TCG_test_case_name(test_cases_name),                         \
                  TCG_test_case_file(test_cases_name),                         \
                  TCG_test_case_line(test_cases_name), LOG_NONE)

#define logstats(test_cases_name, passed_count_int)                            \
    (void)fprintf(stdout, "%s passed %d/%lu\n", #test_cases_name,              \
                  passed_count_int, TCG_tests_count(test_cases_name))

#endif /* LOGGERS_H */
