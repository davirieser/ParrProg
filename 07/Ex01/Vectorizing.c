#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>

#define ENVVAR_SIZE "ENVVAR_SIZE"
#define REPETITIONS 1e6 // Number of repetitions

void multiplyAdd(float* a, const float* b, const float* c, long size) {
    for (int run = 0; run < REPETITIONS; ++run) {
        for (int i = 0; i < size; ++i) {
            a[i] += b[i] * c[i];
        }
    }
}

int main() {

    char* endPtr;
    const long size = strtol(getenv(ENVVAR_SIZE), &endPtr, 10);
    if(*endPtr != '\0'){
        printf("End pointer was not on \\0. Remaining: \"%s\"\n", endPtr);
        return -1;
    }
    float* a = malloc(size * sizeof(float));
    float* b = malloc(size * sizeof(float));
    float* c = malloc(size * sizeof(float));

    // Initialize vectors with constant values
    for (int i = 0; i < size; ++i) {
        a[i] = 0;
        b[i] = 1;
        c[i] = 2;
    }

    // Perform the computation and measure time
    double start = omp_get_wtime();
    multiplyAdd(a, b, c, size);
    double timeTaken = omp_get_wtime() - start;

    // Verify the result
    for (int i = 0; i < size; ++i) {
        if (a[i] != (b[i] * c[i] * REPETITIONS)) {
            printf("Verification failed at index %d\n", i);
            break;
        }
    }

    // Write execution time to file
    FILE* file = fopen("Ex01_time.csv", "a");
    fprintf(file, "%ld, %lf\n", size, timeTaken);
    fclose(file);

    free(a);
    free(b);
    free(c);

    return 0;
}
