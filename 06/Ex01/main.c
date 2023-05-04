
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <omp.h>

#ifndef DEBUG
#define DEBUG 0
#endif

#ifndef PARALLEL
#define PARALLEL 0
#endif

void calc_prefix_sum_serial(int32_t * a, int32_t * b, uint32_t n) {
	b[0] = 0;
	if (n <= 1) { 
		return; 
	}

	int32_t sum = a[0];

	for (uint32_t i = 1; i < n; i ++) {
		b[i] = sum;
		sum += a[i];
	}
}

#define CACHE_LINE_SIZE (64 * (sizeof(long long) / sizeof(char)))
#define CACHE_LINES_PER_THREAD 20

void calc_prefix_sum_parallel(int32_t * a, int32_t * b, uint32_t n) {
	b[0] = 0;
	if (n <= 1) { 
		return; 
	}

	uint32_t num_threads = omp_get_max_threads();
	uint32_t values_per_cache_line = CACHE_LINE_SIZE / sizeof(int32_t);
	uint32_t values_per_thread_iteration = values_per_cache_line * CACHE_LINES_PER_THREAD;
	uint32_t thread_iterations = n / values_per_thread_iteration;

	uint32_t remaining = n - thread_iterations * values_per_thread_iteration;

	if (thread_iterations <= 15) {
		calc_prefix_sum_serial(a, b, n);
		return;
	}

#if DEBUG
	printf("Thread Iters: %d, Val Per Thread Iter: %d, Remaining: %d\n", thread_iterations, values_per_thread_iteration, remaining);
#endif

#pragma omp parallel 
{
	int32_t sum = a[0];
	uint32_t thread_num = omp_get_thread_num();

	for (int i = 0; i < thread_iterations; i ++) {
		if ((i % num_threads) == thread_num) {
			for (int j = 0; j < values_per_thread_iteration; j ++) {
				b[i * values_per_thread_iteration + j + 1] = sum; 
				sum += a[i * values_per_thread_iteration + j]; 
			}
		} else {
			for (int j = 0; j < values_per_thread_iteration; j ++) {
				sum += a[i * values_per_thread_iteration + j]; 
			}
		}
	}
}

	int32_t sum = b[remaining];
	for (int i = remaining; i < n; i ++) {
		b[i] = sum;
		sum += a[i];
	}
}

void print_array(int32_t * arr, char * name, uint32_t n) {
	printf("%s = [", name);
	for (int i = 0; i < n; i ++) {
		printf("%d, ", arr[i]);
	}
	printf("]\n");
}

int main(int argc, char ** argv) {
	if (argc != 2) {
		printf("Usage <%s> [n]\n", argv[0]);
		return EXIT_FAILURE;
	}

	char * ptr;
	long n = strtol(argv[1], &ptr, 10);
	if (*ptr != 0) {
		printf("Could not parse N");
	}

	uint32_t size = n * sizeof(int32_t);
	int32_t * a = malloc(size), * b = malloc(size);

	for (int i = 0; i < n; i ++) {
		a[i] = 1;
	}

#if PARALLEL
	calc_prefix_sum_parallel(a, b, n);
#else
	calc_prefix_sum_serial(a, b, n);
#endif

#if DEBUG
	print_array(a, "a", n);
	print_array(b, "b", n);
#endif

	printf("Last Value : %d", b[n-1]);

	free(a);
	free(b);

	return 0;
}

