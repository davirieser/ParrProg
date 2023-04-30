#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>
#include <time.h>
#include "../../includes/CSVHandler.h"

#define NUM_ITERATIONS 80'000'000

int main() {
    long long i;
    int counter = 0;
    double start_time;

    start_time = omp_get_wtime();

    #pragma omp parallel for
    for (i = 0; i < NUM_ITERATIONS; i++) {
        #pragma omp atomic
        counter++;
    }

    double time = omp_get_wtime() - start_time;

    int numTries = 0;

    FILE* file = fopen("Ex01.csv", "a");
    while(fprintf(file, "%s; %s; %f; %d\n", getenv("OMP_PLACES"), getenv("OMP_PROC_BIND"), time, counter) < 1 && numTries < 10){
        usleep(10000);
        numTries++;
        fseek(file, 0, SEEK_END);
    }
    fclose(file);

    return 0;
}
