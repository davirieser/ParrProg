#include <errno.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifndef NUM_SAMPLES
#define NUM_SAMPLES -1
#endif

#define PERROR fprintf(stderr, "%s:%d: error: %s\n", __FILE__, __LINE__, strerror(errno))
#define PERROR_GOTO(label) \
	do                     \
	{                      \
		PERROR;            \
		goto label;        \
	} while (0)

// Only one malloc for the entire array saves more than 10% for n > 1000
#define INIT_ARRAY(arr, label)            \
	arr = malloc(sizeof(*(arr)) * n * n); \
	if (!(arr))                           \
		PERROR_GOTO(label);

void free_2d_array(int *arr, long len)
{
	if (!arr)
	{
		return;
	}
	free(arr);
}

void printMatrix(int *matrix, int size, char varName)
{
	printf("%c = [", varName);
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			printf("%d.0 ", matrix[i * size + j]);
		}
		printf(";\n");
	}
	printf("]\n\n");
}

int main(int argc, char **argv)
{
		if (NUM_SAMPLES < 0)
	{
		printf("Number of samples was not set.\n");
		return -1;
	}
	long n = NUM_SAMPLES;

	// allocate memory
	int status = EXIT_FAILURE;
	int *a;
	INIT_ARRAY(a, error_a);
	int *b;
	INIT_ARRAY(b, error_b);
	// C is not initialized
	int *c;
	INIT_ARRAY(c, error_c);
	unsigned *local_res = malloc(omp_get_max_threads() * sizeof(*local_res));
	if (!local_res)
		PERROR_GOTO(error_c);
	status = EXIT_SUCCESS;

	// fill matrix
	unsigned int seed = 7;
	// srand(7);

	// #pragma omp parallel for default(none) shared(a, b, n, seed)
	for (long i = 0; i < n; ++i)
	{
		for (long j = 0; j < n; ++j)
		{
			a[i * n + j] = rand_r(&seed) % 255;
			b[i * n + j] = rand_r(&seed) % 255;
			c[i * n + j] = 0;
			// a[i * n + j] = rand();
			// b[i * n + j] = rand();
		}
	}

	// printMatrix(a, n, 'a');
	// printf("\n");
	// printMatrix(b, n, 'b');

	double start_time = omp_get_wtime();
	unsigned long res = 0;

#pragma omp parallel default(none) shared(n, a, b, c, local_res, res)
	{
		// matrix multiplication
		// Speedup by about 1.5% at n = 600 in this section by swapping the order of the loops.
		// parellel for is garbage
// #pragma omp parallel for default(none) shared(n, a, b, c)
#pragma omp for schedule(guided)
		for (long j = 0; j < n; ++j)
		{
			// printf("Thread %d with %ld\n", omp_get_thread_num(), j);
			for (long i = 0; i < n; ++i)
			{
				for (long k = 0; k < n; ++k)
				{
					c[i * n + j] += a[i * n + k] * b[k * n + j];
				}
			}
		}
		// For reduction instead of a parallel partial sum that gets accumulated.
		// Saved about 5-10%
#pragma omp for reduction(+ \
						  : res)
		for (long i = 0; i < n; ++i)
		{
			for (long j = 0; j < n; ++j)
			{
				res += c[i * n + j];
			}
		}
	}
	double end_time = omp_get_wtime();
	printf("res: %lu, time: %2.4f seconds\n", res, end_time - start_time);

	// cleanup
	free(local_res);
error_c:
	free_2d_array(c, n);
error_b:
	free_2d_array(b, n);
error_a:
	free_2d_array(a, n);
	return status;
}