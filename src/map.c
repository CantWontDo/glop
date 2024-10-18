//
// Created by Rahul Pardeshi on 10/16/2024.
//

#include "map.h"

#define MAX_LOAD_FACTOR 0.7
map map_new(u32 n_buckets)
{
    map m =
    {
        .idx_s = 0,
        .n_buckets = n_buckets,
        .f_buckets =  0,
        .key_hash = arr_new_size(u32, n_buckets),
        .vals = arr_new_size(map_entry, n_buckets)
    };

    memset(m.key_hash, 0, sizeof(u32) * n_buckets);
    memset(m.vals, -1, sizeof(map_entry) * n_buckets);
    return m;
}

void map_add(map *m, const void *k, void **arr, const void *elem)
{
    arr_append(arr, elem);
    float load_fac = (float)m->f_buckets / (float)m->n_buckets;

    if (load_fac > MAX_LOAD_FACTOR)
    {
        printf("idx_s: %d, n_buckets: %lu, load_fac: %f rehash!\n", m->idx_s, m->n_buckets, load_fac);
        map_rehash(m);
    }
    u32 hashed = hash(k);
    arr_append(&m->key_hash, &hashed);

    u32 idx = hashed % m->n_buckets;

    while (m->vals[idx].idx != -1)
    {
        idx++;
        if (idx > arr_cap(m->vals))
            idx = 0;
    }

    m->vals[idx] = (map_entry){.idx = m->idx_s, .hash = hashed};

    m->idx_s++;
    m->f_buckets++;
}

u32 map_lookup(map *m, const void *k)
{
    u32 hashed = hash(k);
    u32 idx = hashed % m->n_buckets;

    while (m->vals[idx].hash != hashed)
    {
        idx++;
        if (idx > arr_cap(m->vals))
            idx = 0;
    }

    return m->vals[idx].idx;
}

void map_rehash(map *m)
{
    u32 new_n_buckets = m->n_buckets * 2;
    map_entry *new_vals = arr_new_size(map_entry, new_n_buckets);
    memset(new_vals, -1, sizeof(map_entry) * new_n_buckets);

    for (int i = 0; i < arr_count(m->key_hash); i++)
    {
        u32 hashed = m->key_hash[i];
        u32 idx = hashed % m->n_buckets;
        u32 new_idx = hashed % new_n_buckets;

        while (m->vals[idx].hash != hashed)
        {
            idx++;
            if (idx > m->n_buckets)
                idx = 0;
        }

        while (new_vals[new_idx].idx != -1)
        {
            new_idx++;
            if (new_idx > new_n_buckets)
                new_idx = 0;
        }

        new_vals[new_idx] = m->vals[idx];
    }
    m->vals = new_vals;
    m->n_buckets = new_n_buckets;
}