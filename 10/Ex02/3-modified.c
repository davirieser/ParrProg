
#include <string.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	double start = omp_get_wtime();

	int n = 1 << 24;
	int *x = malloc(n * sizeof(int));
	int *y = malloc(n * sizeof(int));
	int twice = 0;

	memset(x, 0, n);
	memset(y, 0, n);

	x[0] = x[0] + 5 * y[0];
#pragma omp parallel
	{
#pragma omp for
		for (int i = 1; i < n; i++)
		{
			x[i] = x[i] + 5 * y[i];
		}
		if (twice)
		{
#pragma omp for
			for (int i = 1; i < n; i++)
			{
				x[i - 1] = 2 * x[i - 1];
			}
		}
	}

	free(x);
	free(y);
	double timeTaken = omp_get_wtime() - start;
	FILE *file = fopen("Ex02_Time.csv", "a");
	fprintf(file, "3-modified, %d, %f\n", omp_get_max_threads(), timeTaken);
	fclose(file);
}
