#!/usr/bin/env bash

set -euo pipefail

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <problem_name>"
    exit 1
fi

PROBLEM="$1"
DIR="groccc/$PROBLEM"

# Check if directory exists
if [ -d "$DIR" ]; then
    echo "Error: problem '$PROBLEM' already exists at $DIR"
    exit 1
fi

# Struct prefix: Uppercase only the first character
FIRST_CHAR=$(echo "${PROBLEM:0:1}" | tr '[:lower:]' '[:upper:]')
REST_OF_STRING="${PROBLEM:1}"
STRUCT_PREFIX="${FIRST_CHAR}${REST_OF_STRING}"

# Header guard name: all caps, snake stays snake
HEADER_GUARD=$(echo "${PROBLEM}" | tr '[:lower:]' '[:upper:]')

C_FILE="$DIR/${PROBLEM}.c"
H_FILE="$DIR/${PROBLEM}_tests.h"


mkdir -p "$DIR"

###########################################
# Write C file (solution implementation)
###########################################
cat > "$C_FILE" <<EOF
#include "utility/test_case_generator.h"

#include "${PROBLEM}_tests.h"

/* TODO: solve $PROBLEM. Optionally change function signature if desired. */
static struct ${STRUCT_PREFIX}_output
${PROBLEM}(struct ${STRUCT_PREFIX}_input *input)
{
    return (struct ${STRUCT_PREFIX}_output){};
}

int
main(void)
{
    int passed = 0;
    TCG_for_each_test_case(${PROBLEM}_tests, {
        /* TODO: Verify your output against the correct result. */
        struct ${STRUCT_PREFIX}_output const output
            = ${PROBLEM}(&TCG_test_case_input(${PROBLEM}_tests));
        struct ${STRUCT_PREFIX}_output const *const correct_output
            = &TCG_test_case_output(${PROBLEM}_tests);
    });
    return TCG_tests_status(${PROBLEM}_tests, passed);
}
EOF

###########################################
# Write test cases header
###########################################
cat > "$H_FILE" <<EOF
#ifndef ${HEADER_GUARD}_TESTS_H
#define ${HEADER_GUARD}_TESTS_H

#include "utility/test_case_generator.h"

struct ${STRUCT_PREFIX}_input
{
    /* TODO: define input fields */
};

struct ${STRUCT_PREFIX}_output
{
    /* TODO: define expected output fields */
};

TCG_tests_begin(${PROBLEM}_tests,
                struct ${STRUCT_PREFIX}_input,
                struct ${STRUCT_PREFIX}_output)

TCG_test_case("describe this test", {
    .input = {
        /* TODO: struct ${STRUCT_PREFIX}_input field initializers. */
    },
    .output = {
        /* TODO: struct ${STRUCT_PREFIX}_output field initializers. */
    },
})

TCG_tests_end(${PROBLEM}_tests);

#endif /* ${HEADER_GUARD}_TESTS_H */
EOF

#################################
# Append to groccc/CMakeLists.txt
##################################
CMAKELISTS="groccc/CMakeLists.txt"

if [ ! -f "$CMAKELISTS" ]; then
    echo "Error: $CMAKELISTS not found."
    exit 1
fi

echo "add_problem(${PROBLEM})" >> "$CMAKELISTS"

echo "Created problem '$PROBLEM' in $DIR"
echo "Updated $CMAKELISTS"
