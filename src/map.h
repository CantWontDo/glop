//
// Created by Rahul Pardeshi on 10/13/2024.
//

#ifndef MAP_H
#define MAP_H
#include "typedefs.h"
#include "arr.h"
#include "err.h"

typedef struct map
{
    u64 n_buckets;
    u64 *keys;
    u64 *vals;
} map;


#endif //MAP_H
