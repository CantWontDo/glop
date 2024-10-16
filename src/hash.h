//
// Created by Rahul Pardeshi on 10/16/2024.
//

#ifndef HASH_H
#define HASH_H
#include "err.h"
#include "typedefs.h"


static u32 hash(const void *data)
{
    u32 hash_t = 5381;
    char c;
    u8 *data_ = data;

    while ((c = *data_++))
    {
        hash_t = ((hash_t << 5) + hash_t) + c;
    }

    hash_t ^= hash_t >> 16;
    hash_t *= 0x3243f6a9U;
    hash_t ^= hash_t >> 16;
    return hash_t;
}

#endif //HASH_H
