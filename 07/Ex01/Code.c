#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>

#define SIZE 1e5 // Size of the vectors
#define REPETITIONS 1e4 // Number of repetitions

void multiplyAdd(float* a, const float* b, const float* c) {
    for (int run = 0; run < REPETITIONS; ++run) {
        for (int i = 0; i < SIZE; ++i) {
            a[i] += b[i] * c[i];
        }
    }
}

int main() {
    float* a = malloc(SIZE * sizeof(float));
    float* b = malloc(SIZE * sizeof(float));
    float* c = malloc(SIZE * sizeof(float));

    // Initialize vectors with constant values
    for (int i = 0; i < SIZE; ++i) {
        a[i] = 0;
        b[i] = 1;
        c[i] = 2;
    }

    // Perform the computation and measure time
    double start = omp_get_wtime();
    multiplyAdd(a, b, c);
    double timeTaken = omp_get_wtime() - start;

    // Verify the result
    for (int i = 0; i < SIZE; ++i) {
        if (a[i] != (b[i] * c[i] * REPETITIONS)) {
            printf("Verification failed at index %d\n", i);
            break;
        }
    }

    // Write execution time to file
    FILE* file = fopen("Ex01_time.csv", "a");
    fprintf(file, "%lf\n", timeTaken);
    fclose(file);

    free(a);
    free(b);
    free(c);

    return 0;
}
