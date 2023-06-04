
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

	memset(x, 0, n);
	memset(y, 0, n);

#pragma omp parallel for shared(x, y) firstprivate(n)
	for (int i = 0; i < n; i++)
	{
		const double factor = 1.0 / (1 << i);
		x[i] = factor * y[i];
	}

	free(x);
	free(y);
	double timeTaken = omp_get_wtime() - start;
	FILE *file = fopen("Ex02_Time.csv", "a");
	fprintf(file, "1-modified, %d, %f\n", omp_get_max_threads(), timeTaken);
	fclose(file);
}
