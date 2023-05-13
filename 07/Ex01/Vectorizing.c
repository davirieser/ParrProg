#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>

#define ENVVAR_SIZE "ENVVAR_SIZE"
#define ENVVAR_VECTORIZED "ENVVAR_VECTORIZED"
#define REPETITIONS 1e6 // Number of repetitions

#define CHECK_ENDPTR(endPtr)                                               \
    if (*endPtr != '\0')                                                   \
    {                                                                      \
        printf("End pointer was not on \\0. Remaining: \"%s\"\n", endPtr); \
        return -1;                                                         \
    }

void multiplyAdd(float *a, float *b, float *c, long size)
{
    for (int run = 0; run < REPETITIONS; ++run)
    {
        for (int i = 0; i < size; ++i)
        {
            a[i] += b[i] * c[i];
        }
    }
}

int main()
{

    char *endPtr;
    const long SIZE = strtol(getenv(ENVVAR_SIZE), &endPtr, 10);
    CHECK_ENDPTR(endPtr);
    const int VECOTRIZED = strtol(getenv(ENVVAR_VECTORIZED), &endPtr, 10);
    CHECK_ENDPTR(endPtr);
    float *a = malloc(SIZE * sizeof(float));
    float *b = malloc(SIZE * sizeof(float));
    float *c = malloc(SIZE * sizeof(float));

    // Initialize vectors with constant values
    for (int i = 0; i < SIZE; ++i)
    {
        a[i] = 0;
        b[i] = 1;
        c[i] = 2;
    }

    // Perform the computation and measure time
    double start = omp_get_wtime();
    multiplyAdd(a, b, c, SIZE);
    double timeTaken = omp_get_wtime() - start;

    // Verify the result
    int correctResult = 1;
    for (int i = 0; i < SIZE; ++i)
    {
        if (a[i] != (b[i] * c[i] * REPETITIONS))
        {
            correctResult = 0;
            break;
        }
    }

    // Write execution time to file
    FILE *file = fopen("Ex01_time.csv", "a");
    fprintf(file, "%d, %ld, %lf, %d\n", VECOTRIZED, SIZE, timeTaken, correctResult);
    fclose(file);

    free(a);
    free(b);
    free(c);

    return 0;
}
