
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define SIZE 500 

volatile int32_t ** get_matrix(uint32_t size) {
	int32_t * array = (int32_t*) malloc(sizeof(int32_t[size][size]));
	int32_t ** matrix = (int32_t **) malloc(sizeof(int32_t*[size]));

	for (uint32_t i = 0; i < size; i ++) 
		matrix[i] = (int32_t *) (array + (i * size));

	return (volatile int32_t **) matrix;
}

void free_matrix(volatile int32_t ** matrix) {
	free((void *) *matrix);
	free(matrix);
}

int main() {
	volatile size_t n = SIZE;
	volatile int32_t ** a = get_matrix(SIZE), ** b = get_matrix(SIZE), ** c = get_matrix(SIZE);

#if VARIANT == 0
	for (volatile size_t i = 0; i < n; ++i) {
		for (volatile size_t j = 0; j < n; ++j) {
			c[i][j] = a[i][j] * b[i][j];
		}
	}
#else 
	for (size_t j = 0; j < n; ++j) {
		for (size_t i = 0; i < n; ++i) {
			c[i][j] = a[i][j] * b[i][j];
		}
	}
#endif

	free_matrix(a);
	free_matrix(b);
	free_matrix(c);
}
