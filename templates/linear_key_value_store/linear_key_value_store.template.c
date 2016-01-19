/*
 * This template creates a simple key value store.  The key-value pairs are stored in an expandable
 * array in key sorted order.  A key comparison function must be supplied by the user at
 * initialization of the store.  Lookup has O(log(N)) complexity because key are ordered.  Insertion
 * and deletion has complexity O(N) because elements must be moved.  The data structure takes up
 * minimal memory.  This simple key-value store is not well suited for insertion oand deletions in
 * very large data sets.
 *
 *
 * There are three template parameters: NAME, KEY_TYPE, and VALUE_TYPE.
 *
 * The typedefs below are just to make the template file syntactically correct c. It is a cgen comment and will be ignored.
 */

typedef int KEY_TYPE;
typedef int VALUE_TYPE;

// cgen header

#include <stddef.h>
#include <stdbool.h>

struct kv_tuple_NAME {
	KEY_TYPE key;
	VALUE_TYPE value;
};

struct kv_store_NAME {
	int (*cmp)(KEY_TYPE key1, KEY_TYPE key2);
	struct kv_tuple_NAME *data;
	size_t size;
	size_t capacity;
};

struct kv_store_NAME *kv_store_NAME_init(struct kv_store_NAME *store, int (*cmp)(KEY_TYPE key1, KEY_TYPE key2));
void kv_store_NAME_free(struct kv_store_NAME *store);
VALUE_TYPE *kv_store_NAME_get(struct kv_store_NAME *store, KEY_TYPE key);
bool kv_store_NAME_put(struct kv_store_NAME *store, KEY_TYPE key, VALUE_TYPE value);
bool kv_store_NAME_delete(struct kv_store_NAME *store, KEY_TYPE key);

// cgen source

#include <stdlib.h>

struct kv_store_NAME *kv_store_NAME_init(struct kv_store_NAME *store, int (*cmp)(KEY_TYPE key1, KEY_TYPE key2))
{
	store->cmp = cmp;
	store->data = NULL;
	store->size = 0;
	store->capacity = 0;

	return store;
}

void kv_store_NAME_free(struct kv_store_NAME *store)
{
	free(store->data);
}

/* The key is searched in store. lower and upper are the return values. The key woth index lower is
 * less than or equal to key. The key with index upper is greater than or equal to key.  lower is at
 * most upper. lower can be -1 which means that key is below the first element in the store.  upper
 * can be size which means that the key is larger than all keys in the store. The cmp function is
 * used for comparison. If upper == lower, the key is present in the store.
 */

static void kv_store_NAME_search(struct kv_store_NAME *store, KEY_TYPE key, ptrdiff_t *lower, ptrdiff_t *upper)
{
	ptrdiff_t low = -1;
	ptrdiff_t high = store->size;
	ptrdiff_t middle = (low + high) / 2;
	while (middle > low && middle < high) {
		

	}

	*lower = low;
	*upper = high;

	return;
}

/*
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
	TYPE *data;
	if ((data = realloc(vec->data, capacity * sizeof vec->data)) != NULL) {
		vec->data = data;
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
*/
