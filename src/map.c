//
// Created by Rahul Pardeshi on 10/16/2024.
//

#include "map.h"

#include "arr.h"

map map_new(u32 n_buckets)
{
    map m =
    {
        .idx_s = 0,
        .n_buckets = n_buckets,
        .keys = arr_new_size(u32, n_buckets),
        .vals = arr_new_size(i32, n_buckets)
    };

    memset(m.keys, 0, sizeof(u32) * n_buckets);
    memset(m.vals, -1, sizeof(u32) * n_buckets);
    return m;
}

void map_add(map *m, const void *k)
{
    u32 hashed = hash(k);
    arr_add(m->keys, &hashed);

    u32 idx = hashed % m->n_buckets;

    while (m->vals[idx] != -1)
    {
        idx++;
        if (idx > arr_cap(m->vals))
            idx = 0;
    }

    m->vals[idx] = m->idx_s;
    m->idx_s++;
}

u32 map_lookup(map *m, const void *k)
{
    u32 hashed = hash(k);
    u32 idx = hashed % m->n_buckets;

    while (m->vals[idx] == -1)
    {
        idx++;
        if (idx > arr_cap(m->vals))
            idx = 0;
    }

    return m->vals[idx];
}
