
#include <string.h>

int main(int argc, char ** argv) {
	int n = 1024;
	int x[n], y[n];
	int twice = 0;

	memset(x, 0, n);
	memset(y, 0, n);

	x[0] = x[0] + 5 * y[0];
	for (int i = 1; i<n; i++) {
		x[i] = x[i] + 5 * y[i];
		if ( twice ) {
			x[i-1] = 2 * x[i-1];
		}
	}
}

// Dependency Analysis:
// => Assuming x and y are not aliased
// - Anti Dependence on x[0], x[i], x[i-1]
// - Loop carried True Dependency from x[i] to x[i-1]

