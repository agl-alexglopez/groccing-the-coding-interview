#ifndef LOGGERS_H
#define LOGGERS_H

#include <stdio.h> /* NOLINT */

#include "test_case_generator.h"

#define logout(format_string...) (void)fprintf(stdout, format_string);
#define logerr(format_string...) (void)fprintf(stderr, format_string);
#define logfail(test_cases_name)                                               \
    (void)fprintf(stderr, "fail for test: %s, file: %s, line: %d\n",           \
                  TCG_test_case_name(test_cases_name),                         \
                  TCG_test_case_file(test_cases_name),                         \
                  TCG_test_case_line(test_cases_name))

#define logstats(test_cases_name, passed_count_int)                            \
    (void)fprintf(stdout, "%s passed %d/%lu\n", #test_cases_name,              \
                  passed_count_int, TCG_tests_count(test_cases_name))

#endif /* LOGGERS_H */
