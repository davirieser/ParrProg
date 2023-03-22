
#define _GNU_SOURCE

#include "stdlib.h"
#include "stdio.h"
#include "stdint.h"

#include "dlfcn.h"
#include "sys/time.h"

int time(char * program) {
    struct timeval t1, t2;
    double elapsedTime;

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

    int ret = main();

    gettimeofday(&t2, NULL);

    // compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
    printf("Time: %f ms. Return Code: %d\n", elapsedTime, ret);

    return 0;
}

int main(int argc, char ** argv) {
    for (int i = 1; i < argc; i ++) {
        time(argv[i]);
    }
}