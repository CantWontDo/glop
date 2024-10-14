//
// Created by iamro on 10/13/2024.
//

#include "arr.h"

arr_header *arr_get_header(void *arr)
{
    u8* arr_ = arr;
    return (arr_header *)(arr_ - sizeof(arr_header));
}

u8 *arr_header_new(u32 cap, u32 elem_size)
{
    u32 cap_ = 1;
    while (cap_ < cap * elem_size)
        cap_ *= 2;

    arr_header *arr = malloc(sizeof(arr_header) + cap_);
    arr->count = 0;
    arr->cap = cap;
    arr->elem_size = elem_size;

    return (u8 *)arr + sizeof(arr_header);
}

void arr_add(void *arr, const void *elem)
{
    u8 *arr_ = arr;
    arr_header *head = arr_get_header(arr_);

    if (head->count == head->cap)
    {
        head->cap *= 2;
        u32 size = sizeof(arr_header) + head->cap * head->elem_size;
        u8 *new_arr = realloc((u8 *) head, size);

        if (!new_arr)
            log_err("couldn't grow array!");

        head = (arr_header *) new_arr;
        arr_ = new_arr + sizeof(arr_header);
    }

    memcpy(arr_ + head->count * head->elem_size, elem, head->elem_size);
    head->count++;
}

void arr_add_many(void *arr, u32 n_elems, const void *elems)
{
    u8 *arr_ = arr;
    arr_header *head = arr_get_header(arr_);

    u32 new_count = head->count + n_elems;

    if (new_count >= head->cap)
    {
        while(head->cap <= new_count)
            head->cap *= 2;

        u32 size = sizeof(arr_header) + head->cap * head->elem_size;
        u8 *new_arr = realloc((u8 *) head, size);

        if (!new_arr)
            log_err("couldn't grow array!");

        head = (arr_header *) new_arr;
        arr_ = new_arr + sizeof(arr_header);
    }

    memcpy(arr_ + head->count * head->elem_size, elems, n_elems * head->elem_size);
    head->count = new_count;
}

void arr_del(void *arr)
{
    free(arr_get_header(arr));
}

void *arr_end(void *arr)
{
    arr_header* head = arr_get_header(arr);
    return head + head->count * head->elem_size;
}