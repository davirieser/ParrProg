
#include <string.h>

int main(int argc, char ** argv) {
	int n = 1024;
	int x[n], y[n], z[n];

	memset(x, 0, n);
	memset(y, 0, n);
	memset(z, 0, n);

#pragma omp parallel firstprivate(n) shared(x, y, z)
{
#pragma omp for
	for (int i = 1; i<n; i++) {
		y[i] = y[i] + z[i] * 3;
	}
#pragma omp for
	for (int i = 1; i<n; i++) {
		x[i] = (x[i] + y[i-1]) / 2;
	}
}
}
