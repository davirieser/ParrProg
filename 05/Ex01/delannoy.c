
#include <stdio.h>
#include <stdlib.h>

#define SERIAL 0
#define DYNAMIC 1
#define PARALLEL 2

#define VARIANT PARALLEL

#if VARIANT == SERIAL
long delannoy(long n, long m) {
	if (n == 1) 
		return 1 + 2 * m;
	if (m == 1) 
		return 1 + 2 * n;

	if (n == m) {
		return delannoy(n-1, m-1) + 2 * delannoy(n, m-1);
	} else {
		return delannoy(n-1, m-1) + delannoy(n, m-1) + delannoy(n-1, m);
	}	
}
#elif VARIANT == DYNAMIC
long delannoy(long _n, long _m) {
	long n = _n+1, m = _m+1;

	long * arr = malloc(n * m * sizeof(long));
	long ** mem = malloc(n * sizeof(long*)); 

	for (long i = 0; i < n; i ++) {
		mem[i] = &arr[i * m];
	}

	// Initialize upper Edges
	for (long i = 0; i < n; i ++) {
		mem[i][0] = 1;
	}
	for (long i = 0; i < m; i ++) {
		mem[0][i] = 1;
	}

	// Initialize inner Array
	for (long i = 1; i < n; i ++) {
		for (long j = 1; j < m; j ++) {
			mem[i][j] = mem[i][j-1] + mem[(i-1)][(j-1)] + mem[(i-1)][j];
		}	
	}

	long result = mem[n-1][m-1];

	free(mem);
	free(arr);

	return result;
}
#elif VARIANT == PARALLEL
long delannoy(long _n, long _m) {
	long n = _n+1, m = _m+1;

	long * arr = malloc(n * m * sizeof(long));
	long ** mem = malloc(n * sizeof(long*)); 

#pragma omp parrallel 
{
#pragma omp taskloop
	for (long i = 0; i < n; i ++) {
		mem[i] = &arr[i * m];
	}

	// Initialize upper Edges
#pragma omp taskloop
	for (long i = 0; i < n; i ++) {
		mem[i][0] = 1;
	}
#pragma omp taskloop
	for (long i = 0; i < m; i ++) {
		mem[0][i] = 1;
	}

	// Initialize inner Array
#pragma omp taskloop
	for (long i = 1; i < n; i ++) {
		for (long j = 1; j < m; j ++) {
			mem[i][j] = mem[i][j-1] + mem[(i-1)][(j-1)] + mem[(i-1)][j];
		}	
	}

}

	long result = mem[n-1][m-1];

	free(mem);
	free(arr);

	return result;
}
#endif

int main(int argc, char ** argv) {
	long n, m;
	if (argc == 2) {
		char * ptr;

		n = strtol(argv[1], &ptr, 10); 
		if (*ptr != '\0') {
			printf("Could not parse n");
			return EXIT_FAILURE;
		}
		m = n;
	} else if (argc == 3) {
		char * ptr;
		n = strtol(argv[1], &ptr, 10); 
		if (*ptr != '\0') {
			printf("Could not parse n");
			return EXIT_FAILURE;
		}
		m = strtol(argv[2], &ptr, 10); 
		if (*ptr != '\0') {
			printf("Could not parse n");
			return EXIT_FAILURE;
		}
	} else {
		printf("Usage %s <n> [m]", argv[0]);
		return EXIT_FAILURE;
	}

	printf("Delannoy(%ld, %ld) = %ld\n", n, m, delannoy(n, m));

	return 0;
}
