#include <stdio.h>
#include <assert.h>

#include "vector_int.h"

int main(void)
{
	struct vector_int vec;
	vector_int_init(&vec);

	const int N = 1000;
	for (int i = 0; i < N; i++) {
		int value = 10 * i;
		vector_int_append(&vec, value);
	}

	assert(vec.size == N);
	for (int i = 0; i < N; i++) {
		assert(vec.data[i] == 10 * i);
	}

	printf("tests ran succesfully\n");

}
