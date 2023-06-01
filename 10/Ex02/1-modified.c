
#include <string.h>
int main(int argc, char ** argv) {
	int n = 1024;
	int x[n], y[n];

	memset(x, 0, n);
	memset(y, 0, n);

#pragma omp parallel for shared(x, y) firstprivate(n)
	for (int i=0; i < n; i++) {
		const double factor = 1.0 / (1 << i);
		x[i] = factor * y[i];
	}
}
