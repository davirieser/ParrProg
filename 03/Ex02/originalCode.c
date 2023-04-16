#include <errno.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ENVIROMENT_VAR "NUM_SAMPLES"

#define PERROR fprintf(stderr, "%s:%d: error: %s\n", __FILE__, __LINE__, strerror(errno))
#define PERROR_GOTO(label) \
	do                     \
	{                      \
		PERROR;            \
		goto label;        \
	} while (0)

#define INIT_ARRAY(arr, label)                      \
	do                                              \
	{                                               \
		if (!(arr))                                 \
			PERROR_GOTO(label);                     \
		for (long i = 0; i < n; ++i)                \
		{                                           \
			(arr)[i] = malloc(sizeof(**(arr)) * n); \
			if (!(arr)[i])                          \
				PERROR_GOTO(label);                 \
		}                                           \
	} while (0)

void free_2d_array(int **arr, long len)
{
	if (!arr)
	{
		return;
	}
	for (long i = 0; i < len; ++i)
	{
		if (!arr[i])
		{
			break;
		}
		free(arr[i]);
	}
	free(arr);
}

int main(int argc, char **argv)
{
char *ENV_CHAR = getenv(ENVIROMENT_VAR);
	if (ENV_CHAR == NULL)
	{
		printf("Enviroment variable %s was not found.\n", ENVIROMENT_VAR);
		return -1;
	}
	char *endPtr;
	const int NUM_SAMPLES = strtol(ENV_CHAR, &endPtr, 10);
	if (*endPtr != '\0')
	{
		printf("%s could not be converted.\n", ENV_CHAR);
	}
	if (NUM_SAMPLES < 0)
	{
		printf("Number of samples was not set.\n");
		return -1;
	}
	long n = NUM_SAMPLES;

	// allocate memory
	int status = EXIT_FAILURE;
	int **a = malloc(sizeof(*a) * n);
	INIT_ARRAY(a, error_a);
	int **b = malloc(sizeof(*b) * n);
	INIT_ARRAY(b, error_b);
	int **c = malloc(sizeof(*c) * n);
	INIT_ARRAY(c, error_c);
	unsigned *local_res = malloc(omp_get_max_threads() * sizeof(*local_res));
	if (!local_res)
		PERROR_GOTO(error_c);
	status = EXIT_SUCCESS;

	// fill matrix
	srand(7);
	for (long i = 0; i < n; ++i)
	{
		for (long j = 0; j < n; ++j)
		{
			a[i][j] = rand();
			b[i][j] = rand();
		}
	}

	double start_time = omp_get_wtime();
#pragma omp parallel default(none) shared(n, a, b, c, local_res)
	{
		// matrix multiplication
#pragma omp parallel for default(none) shared(n, a, b, c)
		for (long i = 0; i < n; ++i)
		{
			for (long j = 0; j < n; ++j)
			{
				for (long k = 0; k < n; ++k)
				{
					c[i][j] += a[i][k] * b[k][j];
				}
			}
		}

		// sum of matrix c
#pragma omp parallel for default(none) shared(n, a, b, c, local_res)
		for (long i = 0; i < n; ++i)
		{
			for (long j = 0; j < n; ++j)
			{
				local_res[omp_get_thread_num()] += c[i][j];
			}
		}
	}
	unsigned long res = 0;
	for (int l = 0; l < omp_get_num_threads(); ++l)
	{
		res += local_res[l];
	}
	double end_time = omp_get_wtime();

	const char* fileName = "Ex02_CSV_original.csv";
	FILE* file = fopen(fileName, "r");
	int writeHeader = file == NULL;
	if (file != NULL)
	{
		fclose(file);
	}
	file = fopen(fileName, "a");
	if(writeHeader){
		fprintf(file, "NumThreads;Result;executionTime\n");
	}
	// printf("res: %lu, time: %2.4f seconds\n", res, end_time - start_time);
	fprintf(file, "%d;%lu;%6.4f;\n", omp_get_max_threads(), res, end_time - start_time);
	fclose(file);

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