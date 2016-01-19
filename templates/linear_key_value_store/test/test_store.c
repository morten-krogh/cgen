#include <stdio.h>
#include <assert.h>

#include "store_int_int.h"

int compar(int key1, int key2)
{
	return key1 - key2;
}

void print_store(struct kv_store_int_int *store)
{
	printf("\nsize of store = %zu\n", store->size);
	for (size_t i = 0; i < store->size; i++) {
		int key = store->data[i].key;
		int value = store->data[i].value;
		printf("index = %zu, key = %d, value = %d\n", i, key, value);
	}
}

int main(void)
{
	struct kv_store_int_int store;
	kv_store_int_int_init(&store, compar);

	const int N = 100000;
	for (int i = N - 1; i >=0; i--) {
		int key = 10 * i;
		int value = 100 * i;
		kv_store_int_int_put(&store, key, value);
	}

//	print_store(&store);
	
	assert(store.size == N);

	for (int i = 0; i < N; i++) {
		int key = 10 * i;
		int *value = kv_store_int_int_get(&store, key);
		assert(*value == 100 * i);
	}

	assert(NULL == kv_store_int_int_get(&store, 5));

	assert(true == kv_store_int_int_delete(&store, 50));
	assert(store.size == N - 1);

//	print_store(&store);

	assert(false == kv_store_int_int_delete(&store, 50));

	kv_store_int_int_put(&store, 75, 750);
	assert(750 == *kv_store_int_int_get(&store, 75));

//	print_store(&store);

	kv_store_int_int_put(&store, -1, -10);
	kv_store_int_int_put(&store, 999, 9999);
	kv_store_int_int_put(&store, 75, 750);

//	print_store(&store);
	
	assert(store.size == N + 2);	

	kv_store_int_int_free(&store);
	
	printf("tests ran succesfully\n");

}
