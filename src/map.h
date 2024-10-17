//
// Created by Rahul Pardeshi on 10/16/2024.
//

#ifndef MAP_H
#define MAP_H
#include "typedefs.h"
#include "err.h"
#include "hash.h"
#include "arr.h"

typedef struct map
{
    u32 n_buckets;
    u32 f_buckets;
    u32 *keys;
    i32 *vals;

    u32 idx_s;
} map;

map map_new(u32 n_buckets);
void map_add(map *m, const void *k);
u32 map_lookup(map *m, const void *k);

void map_rehash(map *m);
#endif //MAP_H
