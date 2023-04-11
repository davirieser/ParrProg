#include <errno.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main(int argc, char **argv)
{
	// handle input
	if (argc != 2)
	{
		fprintf(stderr, "Error: usage: %s <n>\n", argv[0]);
		return EXIT_FAILURE;
	}
	errno = 0;
	char *str = argv[1];
	char *endptr;
	long n = strtol(str, &endptr, 0);
	if (errno != 0)
	{
		perror("strtol");
		return EXIT_FAILURE;
	}
	if (endptr == str)
	{
		fprintf(stderr, "Error: no digits were found!\n");
		return EXIT_FAILURE;
	}
	if (n < 0)
	{
		fprintf(stderr, "Error: matrix size must not be negative!\n");
		return EXIT_FAILURE;
	}

	// allocate memory
	int status = EXIT_FAILURE;
	int *a;
	INIT_ARRAY(a, error_a);
	int *b;
	INIT_ARRAY(b, error_b);
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
			a[i * n + j] = rand_r(&seed);
			b[i * n + j] = rand_r(&seed);
			// a[i * n + j] = rand();
			// b[i * n + j] = rand();
		}
	}

	double start_time = omp_get_wtime();
#pragma omp parallel default(none) shared(n, a, b, c, local_res)
	{
		// matrix multiplication
		// Speedup by about 1.5% at n = 600 in this section by swapping the order of the loops.
#pragma omp parallel for default(none) shared(n, a, b, c)
		for (long j = 0; j < n; ++j)
		{
			for (long i = 0; i < n; ++i)
			{
				for (long k = 0; k < n; ++k)
				{
					c[i * n + j] += a[i * n + k] * b[k * n + j];
				}
			}
		}

		// sum of matrix c
#pragma omp parallel for default(none) shared(n, a, b, c, local_res)
		for (long i = 0; i < n; ++i)
		{
			for (long j = 0; j < n; ++j)
			{
				local_res[omp_get_thread_num()] += c[i * n + j];
			}
		}
	}
	unsigned long res = 0;
	for (int l = 0; l < omp_get_num_threads(); ++l)
	{
		res += local_res[l];
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