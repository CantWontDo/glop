//
// Created by iamro on 10/13/2024.
//

#include "arr.h"

arr_header *arr_get_header(void *arr)
{
    u8* arr_ = arr;
    return (arr_header *)(arr_ - sizeof(arr_header));
}

u8 *arr_header_new(const u32 cap, const size_t elem_size)
{
    u32 cap_ = 1;
    while (cap_ < cap * elem_size)
        cap_ *= 2;

    arr_header *arr = malloc(sizeof(arr_header) + cap_);
    arr->count = 0;
    arr->cap = cap;
    arr->elem_size = elem_size;

    u8 *arr_ = (u8*)arr + sizeof(arr_header);
    memset(arr_, 0, cap * elem_size);
    return arr_;
}

void arr_append(void **arr_, const void *elem)
{
    u8 **arr = arr_;
    arr_header *head = arr_get_header(*arr);

    if (head->count == head->cap)
    {
        head->cap *= 2;
        u32 size = sizeof(arr_header) + head->cap * head->elem_size;

        u8 *new_arr = realloc((u8*) head, size);
        if (!new_arr)
            log_err("couldn't grow array!");

        head = (arr_header*) new_arr;
        *arr = new_arr + sizeof(arr_header);
    }

    memcpy(*arr + head->count * head->elem_size, elem, head->elem_size);
    memset(*arr + (head->count + 1) * head->elem_size, 0, (head->cap - head->count - 1) * head->elem_size);
    head->count++;
}

void arr_append_many(void **dst, const void *src, const u32 n_elems)
{
    arr_header *head = arr_get_header(*dst);

    u32 new_count = head->count + n_elems;

    if (new_count >= head->cap)
    {
        while (head->cap <= new_count)
            head->cap *= 2;
        u32 size = sizeof(arr_header) + head->cap * head->elem_size;

        u8 *new_arr = realloc((u8*) head, size);
        if (!new_arr)
            log_err("couldn't grow array!");

        head = (arr_header*) new_arr;
        *dst = new_arr + sizeof(arr_header);
    }

    memcpy(*dst + head->count * head->elem_size, src, n_elems * head->elem_size);
    memset(*dst + new_count * head->elem_size, 0, (head->cap - new_count) * head->elem_size);
    head->count = new_count;
}

bool arr_has(void *arr_, const void *elem, size_t elem_size)
{
    u8 *arr = arr_;
    arr_header *head = arr_get_header(arr);

    if (head->elem_size != elem_size)
    {
        log_err("incompatible type");
    }

    for (int i = 0; i < head->count; i++)
    {
        u8 *a = arr + head->elem_size * i;
        if (!memcmp(a, elem, head->elem_size))
        {
          return true;
        }
    }

    return false;
}

void arr_del(void *arr)
{
    free(arr_get_header(arr));
}

void *arr_end(void *arr)
{
    arr_header* head = arr_get_header(arr);
    return (u8*)head + head->count * head->elem_size;
}
