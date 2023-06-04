
#include <string.h>
#include <omp.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	double start = omp_get_wtime();

	int n = 1 << 24;
	int x[n], y[n];
	int twice = 0;

	memset(x, 0, n);
	memset(y, 0, n);

	x[0] = x[0] + 5 * y[0];
	for (int i = 1; i < n; i++)
	{
		x[i] = x[i] + 5 * y[i];
		if (twice)
		{
			x[i - 1] = 2 * x[i - 1];
		}
	}

	double timeTaken = omp_get_wtime() - start;
	FILE *file = fopen("Ex02_Time.csv", "a");
	fprintf(file, "3-original, %d, %f\n", omp_get_num_threads(), timeTaken);
	fclose(file);
}

// Dependency Analysis:
// => Assuming x and y are not aliased
// - Anti Dependence on x[0], x[i], x[i-1]
// - Loop carried True Dependency from x[i] to x[i-1]
