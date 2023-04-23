#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include "../../includes/CSVHandler.h"

#define NUM_ITERATIONS 2'000'000'000

int main() {
    long long i;
    int counter = 0;
    double start_time, end_time;

    start_time = omp_get_wtime();

    #pragma omp parallel for schedule(auto) reduction(+:counter)
    for (i = 0; i < NUM_ITERATIONS; i++) {
        #pragma omp atomic
        counter++;
    }

    end_time = omp_get_wtime();

    FILE* file = fopen("data.csv", "a");
    fprintf(file, "%s, %s, %f, %d\n", getenv("OMP_PLACES"), getenv("OMP_PROC_BIND"), end_time - start_time, counter);
    fclose(file);

    return 0;
}

#ifdef NOT_DEFINED

export OMP_NUM_THREADS=4 && ./threadAffinity

#endif
