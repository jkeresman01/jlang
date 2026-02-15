#pragma once
#include <stdint.h>

// Push element into vector, growing if needed. Returns new data ptr.
void *jlang_vector_push(void *data, int64_t *size, int64_t *capacity, int64_t elem_size, const void *element);

// Reserve capacity. Returns new data ptr.
void *jlang_vector_reserve(void *data, int64_t *capacity, int64_t elem_size, int64_t new_cap);

// Free vector data.
void jlang_vector_free(void *data);
