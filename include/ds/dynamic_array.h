//
// Created by dylanbrass on 2026-11-06.
//

#ifndef DATASTRUCTURES_DYNAMIC_ARRAY_H
#define DATASTRUCTURES_DYNAMIC_ARRAY_H
#include <stddef.h>

typedef struct
{
    void* data;
    size_t type_size;
    size_t length;
    size_t capacity;
} ds_dynamic_array;

int ds_dynamic_array_init(ds_dynamic_array* array, size_t type_size);

void ds_dynamic_array_destroy(ds_dynamic_array* array);

int ds_dynamic_array_add(ds_dynamic_array* array, const void* element);

void* ds_dynamic_array_get(const ds_dynamic_array* array, size_t index);

#endif //DATASTRUCTURES_DYNAMIC_ARRAY_H