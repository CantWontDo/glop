//
// Created by Rahul Pardeshi on 10/16/2024.
//

#include "map.h"

#define MAX_LOAD_FACTOR 0.75
map map_new(u32 n_buckets)
{
    map m =
    {
        .running_idx = 0,
        .n_buckets = n_buckets,
        .f_buckets =  0,
        .key_hash = arr_new_size(u32, n_buckets),
        .entries = arr_new_size(map_entry, n_buckets)
    };

    memset(m.key_hash, 0, sizeof(u32) * n_buckets);
    memset(m.entries, -1, sizeof(map_entry) * n_buckets);
    return m;
}

void map_add(map *m, const void *k, void **arr, const void *elem)
{
    arr_append(arr, elem);
    float load_fac = (float)m->f_buckets / (float)m->n_buckets;

    if (load_fac > MAX_LOAD_FACTOR)
    {
        printf("idx_s: %d, n_buckets: %lu, load_fac: %f rehash!\n", m->running_idx, m->n_buckets, load_fac);
        map_rehash(m);
    }

    u32 hashed = hash(k);
    arr_append(&m->key_hash, &hashed);

    u32 idx = hashed % m->n_buckets;

    // in case of collision, look for a tombstone (-1)
    // TODO: try quadratic probing or some other type later?
    while (m->entries[idx].idx != -1)
    {
        idx++;
        if (idx > arr_cap(m->entries))
            idx = 0;
    }

    // store hash to resolve collision when rehashing
    m->entries[idx] = (map_entry){.idx = m->running_idx, .hash = hashed};

    m->running_idx++;
    m->f_buckets++;
}

i32 map_lookup(map *m, const void *k)
{
    u32 hashed = hash(k);
    if (!arr_has(m->key_hash, &hashed, sizeof(u32)))
    {
        return -1;
    }

    u32 idx = hashed % m->n_buckets;

    // resolve collision by comparing hash
    while (m->entries[idx].hash != hashed)
    {
        idx++;
        if (idx > arr_cap(m->entries))
            idx = 0;
    }

    return m->entries[idx].idx;
}

void map_rehash(map *m)
{
    u32 new_n_buckets = m->n_buckets * 2;
    map_entry *new_entries = arr_new_size(map_entry, new_n_buckets);
    memset(new_entries, -1, sizeof(map_entry) * new_n_buckets);

    for (int i = 0; i < arr_count(m->key_hash); i++)
    {
        u32 hashed = m->key_hash[i];

        // remap
        u32 idx = hashed % m->n_buckets;
        u32 new_idx = hashed % new_n_buckets;

        while (m->entries[idx].hash != hashed)
        {
            idx++;
            if (idx > m->n_buckets)
                idx = 0;
        }

        // look for tombstone to put
        while (new_entries[new_idx].idx != -1)
        {
            new_idx++;
            if (new_idx > new_n_buckets)
                new_idx = 0;
        }

        new_entries[new_idx] = m->entries[idx];
    }
    m->entries = new_entries;
    m->n_buckets = new_n_buckets;
}