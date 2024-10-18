//
// Created by Rahul Pardeshi on 10/16/2024.
//

#include "map.h"

#define MAX_LOAD_FACTOR 0.75
map map_new(u32 n_buckets)
{
    map m =
    {
        .idx_s = 0,
        .n_buckets = n_buckets,
        .f_buckets =  0,
        .keys = arr_new_size(u32, n_buckets),
        .vals = arr_new_size(i32, n_buckets)
    };

    memset(m.keys, 0, sizeof(u32) * n_buckets);
    memset(m.vals, -1, sizeof(i32) * n_buckets);
    return m;
}

void map_add(map *m, const void *k)
{
    float load_fac = m->f_buckets / m->n_buckets;

    if (load_fac > MAX_LOAD_FACTOR)
        map_rehash(m);

    u32 hashed = hash(k);
    arr_append(&m->keys, &hashed);

    u32 idx = hashed % m->n_buckets;

    while (m->vals[idx] != -1)
    {
        idx++;
        if (idx > arr_cap(m->vals))
            idx = 0;
    }

    m->vals[idx] = m->idx_s;
    m->idx_s++;
    m->f_buckets++;
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

void map_rehash(map *m)
{
    u32 new_n_buckets = m->n_buckets * 2;
    i32 *new_vals = arr_new_size(i32, new_n_buckets);
    memset(new_vals, -1, sizeof(i32) * new_n_buckets);

    arr_append_many(&m->keys, 0, m->n_buckets);

    for (int i = 0; i < m->n_buckets; i++)
    {
        u32 hashed = m->keys[i];

        if (hashed != 0)
        {
            u32 idx = hashed % m->n_buckets;
            u32 new_idx = hashed % new_n_buckets;

            while (m->vals[idx] == -1)
            {
                idx++;
                if (idx > arr_cap(m->vals))
                    idx = 0;
            }

            while (new_vals[new_idx] != -1)
            {
                new_idx++;
                if (new_idx > new_n_buckets)
                    new_idx = 0;
            }

            new_vals[new_idx] = m->vals[idx];
        }
    }

    m->vals = new_vals;
    m->n_buckets = new_n_buckets;
}