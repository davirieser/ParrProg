
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define SERIAL 0
#define DYNAMIC 1
#define PARALLEL 2

#define ENVVAR_VARIANT "ENVVAR_VARIANT"
#define ENVVAR_SIZE_M "ENVVAR_SIZE_M"
#define ENVVAR_SIZE_N "ENVVAR_SIZE_N"

long delannoy_serial(long n, long m)
{
	if (n == 1)
		return 1 + 2 * m;
	if (m == 1)
		return 1 + 2 * n;

	if (n == m)
	{
		return delannoy_serial(n - 1, m - 1) + 2 * delannoy_serial(n, m - 1);
	}
	else
	{
		return delannoy_serial(n - 1, m - 1) + delannoy_serial(n, m - 1) + delannoy_serial(n - 1, m);
	}
}

long delannoy_dynamic(long _n, long _m)
{
	long n = _n + 1, m = _m + 1;

	long *arr = malloc(n * m * sizeof(long));
	long **mem = malloc(n * sizeof(long *));

	for (long i = 0; i < n; i++)
	{
		mem[i] = &arr[i * m];
	}

	// Initialize upper Edges
	for (long i = 0; i < n; i++)
	{
		mem[i][0] = 1;
	}
	for (long i = 0; i < m; i++)
	{
		mem[0][i] = 1;
	}

	// Initialize inner Array
	for (long i = 1; i < n; i++)
	{
		for (long j = 1; j < m; j++)
		{
			mem[i][j] = mem[i][j - 1] + mem[(i - 1)][(j - 1)] + mem[(i - 1)][j];
		}
	}

	long result = mem[n - 1][m - 1];

	free(mem);
	free(arr);

	return result;
}

long delannoy_parallel(long _n, long _m)
{
	long n = _n + 1, m = _m + 1;

	long *arr = malloc(n * m * sizeof(long));
	long **mem = malloc(n * sizeof(long *));

#pragma omp parallel
	{
#pragma omp taskloop
		for (long i = 0; i < n; i++)
		{
			mem[i] = &arr[i * m];
		}

		// Initialize upper Edges
#pragma omp taskloop
		for (long i = 0; i < n; i++)
		{
			mem[i][0] = 1;
		}
#pragma omp taskloop
		for (long i = 0; i < m; i++)
		{
			mem[0][i] = 1;
		}

		// Initialize inner Array
#pragma omp taskloop
		for (long i = 1; i < n; i++)
		{
			for (long j = 1; j < m; j++)
			{
				mem[i][j] = mem[i][j - 1] + mem[(i - 1)][(j - 1)] + mem[(i - 1)][j];
			}
		}
	}

	long result = mem[n - 1][m - 1];

	free(mem);
	free(arr);

	return result;
}

#define SAVE_STRTOL(charPtr, endPtr, resultVar)                                \
	if (charPtr == NULL)                                                       \
	{                                                                          \
		printf("%d - %s was NULL\n", __LINE__, #charPtr);                      \
		return EXIT_FAILURE;                                                   \
	}                                                                          \
	resultVar = strtol(charPtr, &endPtr, 10);                                  \
	if (*endPtr != '\0')                                                       \
	{                                                                          \
		printf("Could not parse \"%s\", \"%s\" remained.\n", charPtr, endPtr); \
		return EXIT_FAILURE;                                                   \
	}

int main()
{
	char *env_variant = getenv(ENVVAR_VARIANT);
	char *env_m = getenv(ENVVAR_SIZE_M);
	char *env_n = getenv(ENVVAR_SIZE_N);
	long n, m;
	int variant;
	char *endPtr;

	SAVE_STRTOL(env_variant, endPtr, variant);
	SAVE_STRTOL(env_m, endPtr, m);
	SAVE_STRTOL(env_n, endPtr, n);

	if (m < 0)
	{
		printf("m was out of range. Value was %ld\n", m);
		return EXIT_FAILURE;
	}
	if (n < 0)
	{
		printf("n was out of range. Value was %ld\n", n);
		return EXIT_FAILURE;
	}


	long (*delannoyFuncton)(long _n, long _m);
	switch (variant)
	{
	case SERIAL:
		delannoyFuncton = delannoy_serial;
		break;
	case DYNAMIC:
		delannoyFuncton = delannoy_dynamic;
		break;
	case PARALLEL:
		delannoyFuncton = delannoy_parallel;
		break;
	default:
		printf("Variant was out of range. Value was %d\n", variant);
		return EXIT_FAILURE;
		break;
	}
	double startTime = omp_get_wtime();
	delannoyFuncton(n, m);
	double timeTaken = omp_get_wtime() - startTime;

	FILE* file = fopen("Ex01.csv", "a");
	fprintf(file, "%d;%d;%ld;%ld;%lf\n", variant, omp_get_max_threads(), m, n, timeTaken);
	fclose(file);
	// printf("Delannoy(%ld, %ld) = %ld\n", n, m, delannoyFuncton(n, m));

	return 0;
}
