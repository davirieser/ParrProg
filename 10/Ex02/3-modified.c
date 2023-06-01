
#include <string.h>

int main(int argc, char ** argv) {
	int n = 1024;
	int x[n], y[n];
	int twice = 0;

	memset(x, 0, n);
	memset(y, 0, n);

	x[0] = x[0] + 5 * y[0];
#pragma omp parallel 
{
#pragma omp for
	for (int i = 1; i<n; i++) {
		x[i] = x[i] + 5 * y[i];
	}
	if ( twice ) {
#pragma omp for
		for (int i = 1; i<n; i++) {
				x[i-1] = 2 * x[i-1];
			}
		}
	}
}
