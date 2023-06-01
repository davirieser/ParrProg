
#include <string.h>

int main(int argc, char ** argv) {
	int n = 1024, a = 1024, b = 1024;
	int x[n], y[n];

	memset(x, 0, n);
	memset(y, 0, n);

	for (int i=0; i < n; i++) {
	   x[i] = y[i] * 2 + b * i;
	}

	for (int i=0; i < n; i++) {
	   y[i] = x[i]  + a / (i+1);
	}
}

// Dependency Analysis:
// If x and y are not aliased:
// - True Dependency for x[i]
// - Anti Dependence for y[i]
// If x and y are aliased:
// - Anti Dependence in first and second loop
// - Output Dependence between loops

