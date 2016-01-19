/* This file is generated by the cgen program, https://github.com/morten-krogh/cgen. The cgen program is released under the MIT license.*/

#include <stddef.h>
#include <stdbool.h>

struct kv_tuple_int_int {
	int key;
	int value;
};

struct kv_store_int_int {
	int (*compar)(int key1, int key2);
	struct kv_tuple_int_int *data;
	size_t size;
	size_t capacity;
};

struct kv_store_int_int *kv_store_int_int_init(struct kv_store_int_int *store, int (*compar)(int key1, int key2));
void kv_store_int_int_free(struct kv_store_int_int *store);
int *kv_store_int_int_get(struct kv_store_int_int *store, int key);
bool kv_store_int_int_put(struct kv_store_int_int *store, int key, int value);
bool kv_store_int_int_delete(struct kv_store_int_int *store, int key);