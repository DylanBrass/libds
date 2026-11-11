//
// Created by dylanbrass on 2026-11-06.
//

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <ds/dynamic_array.h>

int ds_dynamic_array_init(ds_dynamic_array* array, const size_t type_size)
{
    if (array == nullptr || type_size == 0)
    {
        return -1;
    }

    array->type_size = type_size;
    array->length = 0;
    array->capacity = 1;

    array->data = malloc(array->capacity * array->type_size);
    if (array->data == nullptr)
    {
        array->capacity = 0;
        return -1;
    }

    return 0;
}

void* ds_dynamic_array_get(const ds_dynamic_array* array, const size_t index)
{
    if (array == nullptr || index >= array->length)
    {
        return nullptr;
    }

    return (char*)array->data + index * array->type_size;
}

int ds_dynamic_array_add(ds_dynamic_array* array, const void* element)
{
    if (array == nullptr || element == nullptr)
    {
        return -1;
    }

    if (array->length == array->capacity)
    {
        const size_t new_capacity = array->capacity == 0 ? 1 : array->capacity * 2;

        if (new_capacity > SIZE_MAX / array->type_size)
        {
            return -1;
        }

        void* new_data = realloc(array->data, new_capacity * array->type_size);
        if (new_data == nullptr)
        {
            return -1;
        }

        array->data = new_data;
        array->capacity = new_capacity;
    }

    // Go to the end of the dynamic array
    unsigned char* dest = (unsigned char*)array->data + array->length * array->type_size;
    // Take the bytes from the element, this will be the size of the type.
    const unsigned char* src = element;

    // copy the bytes
    for (size_t i = 0; i < array->type_size; i++)
    {
        dest[i] = src[i];
    }

    array->length++;

    return 0;
}

void ds_dynamic_array_destroy(ds_dynamic_array* array)
{
    if (array == nullptr)
    {
        return;
    }

    free(array->data);
    array->data = nullptr;
    array->length = 0;
    array->capacity = 0;
}
