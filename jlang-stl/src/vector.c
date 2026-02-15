#include <jlang_stl/vector.h>
#include <stdlib.h>
#include <string.h>

void *jlang_vector_push(void *data, int64_t *size, int64_t *capacity,
                         int64_t elem_size, const void *element)
{
    if (*size == *capacity)
    {
        int64_t new_cap = *capacity == 0 ? 8 : *capacity * 2;
        data = realloc(data, new_cap * elem_size);
        *capacity = new_cap;
    }
    memcpy((char *)data + (*size) * elem_size, element, elem_size);
    (*size)++;
    return data;
}

void *jlang_vector_reserve(void *data, int64_t *capacity,
                            int64_t elem_size, int64_t new_cap)
{
    if (new_cap > *capacity)
    {
        data = realloc(data, new_cap * elem_size);
        *capacity = new_cap;
    }
    return data;
}

void jlang_vector_free(void *data)
{
    free(data);
}
