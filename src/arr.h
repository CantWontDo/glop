//
// Created by Rahul Pardeshi on 10/13/2024.
//

#ifndef ARR_H
#define ARR_H

#include "typedefs.h"
#include "err.h"

#include <string.h>

typedef struct arr_header
{
    u32 count;
    u32 cap;
    u32 elem_size;
} arr_header;

arr_header *arr_get_header(void *arr);
u8 *arr_header_new(u32 cap, u32 elem_size);

void arr_add(void *arr, void *elem);
void arr_add_many(void *arr, u32 n_elems, void *elems);

void arr_del(void *arr);

void *arr_end(void *arr);

#define arr_new(type, arr_size) ((type*) arr_header_new(arr_size, sizeof(type)))
#define arr_count(arr) (arr_get_header(arr)->count)
#define arr_cap(arr) (arr_get_header(arr)->cap)

#define arr_clear(arr) (arr_get_header(arr)->count = 0)
#endif //ARR_H
