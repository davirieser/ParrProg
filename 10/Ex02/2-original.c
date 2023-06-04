
#include <string.h>
#include <omp.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	double start = omp_get_wtime();

	int n = 1 << 24;
	int x[n], y[n], z[n];

	memset(x, 0, n);
	memset(y, 0, n);
	memset(z, 0, n);

	for (int i = 1; i < n; i++)
	{
		x[i] = (x[i] + y[i - 1]) / 2;
		y[i] = y[i] + z[i] * 3;
	}

	double timeTaken = omp_get_wtime() - start;
	FILE *file = fopen("Ex02_Time.csv", "a");
	fprintf(file, "2-original, %d, %f\n", omp_get_num_threads(), timeTaken);
	fclose(file);
}

// Dependency Analysis:
// => Assumption: Arrays are not aliased
// - Anti Dependence on x[i]
// - Loop carried True Dependency from y[i] to y[i-1]
// - Anti Dependency on y[i]
