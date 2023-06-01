
#include <stdio.h>
#include <string.h>
#include <math.h>

int main(int argc, char ** argv) {
	int n = 1024, f, k = 1024;
	int x[n], y[n], z[n];

	memset(x, 0, n);
	memset(y, 0, n);
	memset(z, 0, n);

#pragma omp parallel for shared(n, x, y, z) private(f, k)
	for (int i=0; i < n; i++) {
		// Removed unnecessary temporary variable a
		z[i] = (x[i] + y[i]) / (i+1);
	}

	// Used last value in z-Array as a instead
	f = sqrt(z[n-1] + k);

	printf("%d", f);
}
