
#include <string.h>
#include <omp.h>
#include <stdio.h>

int main(int argc, char **argv)
{

	double start = omp_get_wtime();

	int n = 10240;
	int x[n], y[n];

	memset(x, 0, n);
	memset(y, 0, n);

#pragma omp parallel for shared(x, y) firstprivate(n)
	for (int i = 0; i < n; i++)
	{
		const double factor = 1.0 / (1 << i);
		x[i] = factor * y[i];
	}

	double timeTaken = omp_get_wtime() - start;
	FILE *file = fopen("Ex02_Time.csv", "a");
	fprintf(file, "1-modified, %d, %f\n", omp_get_num_threads(), timeTaken);
	fclose(file);
}
