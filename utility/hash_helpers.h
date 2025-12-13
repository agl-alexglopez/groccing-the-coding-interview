#ifndef HASH_HELPERS_H
#define HASH_HELPERS_H

#include <stdint.h>

#include "ccc/types.h"
#include "str_view/str_view.h"

/*================================  Common Types   ==========================*/

struct Int_key_val
{
    int key;
    int val;
};

/*================================  CCC Callbacks   =========================*/

CCC_Order hash_map_int_key_val_order(CCC_Key_comparator_context order);
uint64_t hash_map_int_to_u64(CCC_Key_context k);

/*================================  Inline Helpers  =========================*/

/** @brief An inline utility hasher for arbitrary hashing and safe aliasing.
@param[in] str a view of read only const char data.
@return the 64 bit hash value for the input bytes.
@note Consider using this for strings or aliasing any data of variable length
and type.
@warning If the underlying data is not null terminated, be sure to set the
correct length for the SV_Str_view.

It is safe to alias any data with this function. The C language specifies that
any type may be safely aliased by `char *` and `char const *`. This hash
function also only iterates over the range provided so it is safe to provide
non-null terminated data to this function (even though the SV_Str_view library
says this is not recommended). */
static inline uint64_t
hash_fnv_1a_str_view_to_u64(SV_Str_view const str)
{
    /* 64 bit Offset basis. Changes based on return bit width. */
    uint64_t hash = 14695981039346656037ULL;
    for (char const *c = SV_begin(str); c != SV_end(str); c = SV_next(c))
    {
        hash ^= *c;
        /* Magic is FNV prime number specific to 64 bit width. */
        hash *= 1099511628211ULL;
    }
    return hash;
}

#endif /* HASH_HELPERS_H */
