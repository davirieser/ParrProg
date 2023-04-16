
#include "stdio.h"

int setup() {
	long i = 100;
	return i;
}

int run(void * args) {
	perror("Args: %ld\n");
	return 0;
}
