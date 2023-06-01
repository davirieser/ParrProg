
#include <stdio.h>
#include <string.h>
#include <math.h>

int main(int argc, char ** argv) {
	int a = 1024, n = 1024, f, k = 1024;
	int x[n], y[n], z[n];

	memset(x, 0, n);
	memset(y, 0, n);
	memset(z, 0, n);

	for (int i=0; i < n; i++) {
		a = (x[i] + y[i]) / (i+1);
		z[i] = a;
	}

	f = sqrt(a + k);
}

// Dependency Analysis:
// - True Dependence from "a = " to "= a" 
// - True Dependence from "a = " to "sqrt(a + k)"
// If x or y are aliased with z:
// - Anti Dependence from x[i] or y[i] to z[i]

