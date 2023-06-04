
#include <string.h>
#include <omp.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	double start = omp_get_wtime();

	int n = 1 << 24;
	int x[n], y[n];
	double factor = 1;

	memset(x, 0, n);
	memset(y, 0, n);

	for (int i = 0; i < n; i++)
	{
		x[i] = factor * y[i];
		factor = factor / 2;
	}

	double timeTaken = omp_get_wtime() - start;
	FILE *file = fopen("Ex02_Time.csv", "a");
	fprintf(file, "1-original, %d, %f\n", omp_get_num_threads(), timeTaken);
	fclose(file);
}

// Dependency Analysis:
// - Loop carried True dependence on factor
// - Loop independent Anti dependence on factor
//		If y and x are aliased:
//		- True Dependence between x[i] and y[i]
