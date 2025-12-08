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
STRUCT_PREFIX="$(printf "%s" "$PROBLEM" | sed -E 's/^([a-z])/\U\1/')"

# Header guard name: all caps, snake stays snake
HEADER_GUARD="$(printf "%s" "$PROBLEM" | tr '[:lower:]' '[:upper:]')_TEST_CASES_H"

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
#ifndef ${HEADER_GUARD}
#define ${HEADER_GUARD}

#include "../test_case_generator.h"

struct ${STRUCT_PREFIX}_input
{
    /* TODO: define input fields */
};

struct ${STRUCT_PREFIX}_output
{
    /* TODO: define expected output fields */
};

/* clang-format off */
TCG_tests_begin(${PROBLEM}_tests,
                struct ${STRUCT_PREFIX}_input,
                struct ${STRUCT_PREFIX}_output)

TCG_test_case_begin("describe this test", {
    /* input struct designated initializer fields */
})
TCG_test_case_end({
    /* output struct designated initializer fields */
})

TCG_tests_end(${PROBLEM}_tests);
/* clang-format on */

#endif /* ${HEADER_GUARD} */
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
