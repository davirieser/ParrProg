
#include <string.h>
int main(int argc, char ** argv) {
	int n = 1024;
	int x[n], y[n];
	double factor = 1;

	memset(x, 0, n);
	memset(y, 0, n);

	for (int i=0; i < n; i++) {
		x[i] = factor * y[i];
		factor = factor / 2;
	}
}

// Dependency Analysis:
// - Loop carried True dependence on factor
// - Loop independent Anti dependence on factor
//		If y and x are aliased:
//		- True Dependence between x[i] and y[i]
