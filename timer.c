
#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <fcntl.h>
#include <unistd.h>
#include "dlfcn.h"
#include "sys/time.h"
#include "time.h"

int time_program(char * program) {
    struct timeval t1, t2;
	clock_t start_t, end_t;
    double elapsed_time, cpu_time;

    // Try to open the Library
    void * handle = dlopen(program, RTLD_LAZY);
    if (handle == NULL) {
        printf(
            "Error while loading shared libraries: %s: cannot open shared object file: No such file or directory",
            program
        );
        return -1;
    }

    /* Source for the Error Handling: Man Page dlopen */
    dlerror();    /* Clear any existing error */

    int (* main)(void) = dlsym(handle, "main");

    /* Check if an actual Error occured */
    char * error = dlerror();
    if (error != NULL) {
        printf(
            "Symbol lookup error: Could not find Main Function\n"
        );
        return -1;
    }

    gettimeofday(&t1, NULL);
	start_t = clock();

    int ret = main();

	end_t = clock();
    gettimeofday(&t2, NULL);

	cpu_time = (double)(end_t - start_t) / CLOCKS_PER_SEC;

    // compute and print the elapsed time in millisec
    elapsed_time = (t2.tv_sec - t1.tv_sec);  
    elapsed_time += ((double) (t2.tv_usec - t1.tv_usec)) / 10000000;

	int pfd;
	if ((pfd = open("log.csv", O_APPEND | O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1) {
		perror("Cannot open output file\n"); 
		return EXIT_FAILURE;
	}

	char * str;
	int len = asprintf(&str, "%s, %f s, %f s, %d\n", program, cpu_time, elapsed_time, ret);
	if (write(pfd, str, len) == -1) {
		perror("Could not write to Output File\n"); 
		return EXIT_FAILURE;
	}

	free(str);
	close(pfd);

    return 0;
}

int main(int argc, char ** argv) {
    for (int i = 1; i < argc; i ++) {
        time_program(argv[i]);
    }
}
