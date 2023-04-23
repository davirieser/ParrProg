#include <stdio.h>
#include <omp.h>
#include <time.h>

#define NUM_ITERATIONS 1'000'000'000

int main() {
    long long i;
    long long counter = 0;
    double start_time, end_time;

    start_time = omp_get_wtime();

    #pragma omp parallel for schedule(auto) reduction(+:counter)
    for (i = 0; i < NUM_ITERATIONS; i++) {
        #pragma omp atomic
        counter++;
    }

    end_time = omp_get_wtime();

    printf("Time taken = %f seconds\n", end_time - start_time);
    printf("Counter value = %lld\n", counter);

    return 0;
}

#ifdef NOT_DEFINED

export OMP_NUM_THREADS=4 && ./threadAffinity

#endif
