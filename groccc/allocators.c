#include <stddef.h>
#include <stdlib.h>

#include "allocators.h"

#include "ccc/types.h"

void *
stdlib_allocate(CCC_Allocator_context const context)
{
    if (!context.input && !context.bytes)
    {
        return NULL;
    }
    if (!context.input)
    {
        return malloc(context.bytes);
    }
    if (!context.bytes)
    {
        free(context.input);
        return NULL;
    }
    return realloc(context.input, context.bytes);
}
