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
    size_t elem_size;
} arr_header;

arr_header *arr_get_header(void *arr);
u8 *arr_header_new(const u32 cap, const size_t elem_size);

void arr_append(void **arr, const void *elem);
void arr_append_many(void **dst_, const void *src, const u32 n_elems);

void arr_grow(void **arr);

void arr_del(void *arr);

void *arr_end(void *arr);

char *arr_to_str(void *arr);

bool arr_has(void *arr, const void *elem, size_t elem_size);

#define arr_new_size(type, arr_size) ((type *) arr_header_new(arr_size, sizeof(type)))
#define arr_new(type) ((arr_new_size(type, 1)))
#define arr_arr_new(type)

#define arr_count(arr) (arr_get_header(arr)->count)
#define arr_cap(arr) (arr_get_header(arr)->cap)
#define arr_elem_size(arr) (arr_get_header(arr)->elem_size)

#define arr_clear(arr) (arr_get_header(arr)->count = 0)
#endif //ARR_H
