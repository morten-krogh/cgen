// This template creates a data structure called vector. A vector is a dynamic array.

// There are two template parameters: NAME and TYPE.

// The typedef below is just to make the template file syntactically correct c. It is a cgen comment and will be ignored.
// Syntactically correct cgen template files are easier to write in standard editors and can be syntactically verified by a c compiler.
typedef int TYPE;

// cgen header file include

#include <stddef.h>

struct vector_NAME {
       TYPE *data;
       size_t size;
       size_t capacity;
};

// cgen source file include

#include <stdlib.h>
// cgen header file declarations and source file definitions

struct vector_NAME *vector_NAME_init(struct vector_NAME *vec)
{
	vec->data = NULL;
	vec->size = 0;
	vec->capacity = 0;

	return vec;
}

void vector_NAME_free(struct vector_NAME *vec)
{
	free(vec->data);
}

struct vector_NAME *vector_NAME_set_capacity(struct vector_NAME *vec, size_t capacity)
{
	if (realloc(vec->data, capacity) != NULL) {
		vec->capacity = capacity;
		if (vec->size > capacity) vec->size = capacity;
	}

	return vec;
}

struct vector_NAME *vector_NAME_append(struct vector_NAME *vec, TYPE t)
{
	if (vec->size == vec->capacity) {
		size_t new_capacity = 2 * vec->capacity + 1;
		vector_NAME_set_capacity(vec, new_capacity);
	}

	vec->data[vec->size] = t;
	vec->size++;
	
	return vec;
}





