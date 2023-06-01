
#include <string.h>

int main(int argc, char ** argv) {
	int n = 1024, a = 1024, b = 1024;
	int x[n], y[n];

	memset(x, 0, n);
	memset(y, 0, n);

#pragma omp parallel for shared(n, x, y) firstprivate(a, b)
	for (int i=0; i < n; i++) {
	   x[i] = y[i] * 2 + b * i;
	   y[i] = x[i] + a / (i+1);
	}
}
