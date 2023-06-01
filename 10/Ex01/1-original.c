
#include <string.h>

int main(int argc, char ** argv) {
	int n = 1024;
	int x[n], y[n];

	memset(x, 0, n);
	memset(y, 0, n);

	for (int i=0; i < n-1; i++) {
		x[i] = (y[i] + x[i+1]) / 7;
	}
}

// Dependency Analysis:
// - If y and x are not aliased:
//		Loop carried Anti Dependence from x[i+1] to x[i]
// - If y and x are aliased:
//		Loop carried Anti Dependence from x[i+1] to x[i] and
//		Anti Dependence Dependence from y[i] to x[i]
//		
// I don't think this loop can be parallelized in a good way.
// It would require copying the x-Array.

