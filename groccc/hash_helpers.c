#include <stdint.h>

#include "ccc/types.h"
#include "hash_helpers.h"
#include "str_view/str_view.h"

CCC_Order
hash_map_int_key_val_order(CCC_Key_comparator_context const order)
{
    struct Int_key_val const *const right = order.type_right;
    int const left = *((int *)order.key_left);
    return (left > right->key) - (left < right->key);
}

uint64_t
hash_map_int_to_u64(CCC_Key_context const k)
{
    int const id_int = *((int *)k.key);
    uint64_t x = id_int;
    x = (x ^ (x >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
    x = (x ^ (x >> 27)) * UINT64_C(0x94d049bb133111eb);
    x = x ^ (x >> 31);
    return x;
}

uint64_t
hash_fnv_1a_str_view_to_u64(CCC_Key_context const k)
{
    SV_Str_view str = *(SV_Str_view *)k.key;
    /* 64 bit Offset basis. Changes based on return bit width. */
    uint64_t hash = 14695981039346656037ULL;
    for (char const *c = SV_begin(str); c != SV_end(str); c = SV_next(c))
    {
        hash ^= *c;
        /* Magic is FNV prime number specific to 64 bit width. */
        hash = hash * 1099511628211ULL;
    }
    return hash;
}
