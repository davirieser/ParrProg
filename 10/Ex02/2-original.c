
#include <string.h>

int main(int argc, char ** argv) {
	int n = 1024;
	int x[n], y[n], z[n];

	memset(x, 0, n);
	memset(y, 0, n);
	memset(z, 0, n);

	for (int i = 1; i<n; i++) {
		x[i] = (x[i] + y[i-1]) / 2;
		y[i] = y[i] + z[i] * 3;
	}
}

// Dependency Analysis:
// => Assumption: Arrays are not aliased
// - Anti Dependence on x[i]
// - Loop carried True Dependency from y[i] to y[i-1]
// - Anti Dependency on y[i]

