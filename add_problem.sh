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
H_FILE="$DIR/${PROBLEM}_test_cases.h"


mkdir -p "$DIR"

###########################################
# Write C file (solution implementation)
###########################################
cat > "$C_FILE" <<EOF
#include "${PROBLEM}_test_cases.h"

/* TODO: solve $PROBLEM */
static struct ${STRUCT_PREFIX}_output
${PROBLEM}(struct ${STRUCT_PREFIX}_input *input)
{
    return (struct ${STRUCT_PREFIX}_output){};
}

int
main(void)
{
    TCG_for_each_test_case(${PROBLEM}_tests, {
        /* TODO: Use these to check your output against the correct output. */
        struct ${STRUCT_PREFIX}_output const output 
            = ${PROBLEM}(&TCG_test_case_input(${PROBLEM}_tests));
        struct ${STRUCT_PREFIX}_output const *const correct_output 
            = &TCG_test_case_output(${PROBLEM}_tests);
    });
    return 0;
}
EOF

###########################################
# Write test cases header
###########################################
cat > "$H_FILE" <<EOF
#ifndef ${HEADER_GUARD}_H
#define ${HEADER_GUARD}_H

#include "../test_case_generator.h"

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
        /* struct ${STRUCT_PREFIX}_input field initializers. */
    },
    .output = {
        /* struct ${STRUCT_PREFIX}_output field initializers. */
    },
})

TCG_tests_end(${PROBLEM}_tests);

#endif /* ${HEADER_GUARD}_H */
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
