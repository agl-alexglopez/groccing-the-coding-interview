#ifndef HASH_HELPERS_H
#define HASH_HELPERS_H

#include <stdint.h>

#include "ccc/types.h"

struct Int_key_val
{
    int key;
    int val;
};

CCC_Order hash_map_int_key_val_order(CCC_Key_comparator_context order);
uint64_t hash_map_int_to_u64(CCC_Key_context k);
uint64_t hash_fnv_1a_str_view_to_u64(CCC_Key_context k);

#endif /* HASH_HELPERS_H */
