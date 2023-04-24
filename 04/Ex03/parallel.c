
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#define ARRAY_SIZE 100000000

typedef struct mem
{
    int *a;
    int *b;
} mem_t;

// Source: https://rosettacode.org/wiki/Binary_search#C

int binary_search(int *a, int n, int x)
{
    int i = 0, j = n - 1, k = 0;
    while (i <= j)
    {
        k = i + ((j - i) / 2);
        if (a[k] == x)
        {
            return k;
        }
        else if (a[k] < x)
        {
            i = k + 1;
        }
        else
        {
            j = k - 1;
        }
    }
    return i;
}

// Source: Lecture Slides

void p_merge(int *c, int *a, int *b, int na, int nb)
{
    if (na < nb)
    {
        p_merge(c, b, a, nb, na);
    }
    else if (na == 0)
    {
        return;
    }
    else
    {
        int ma = na / 2;
        int mb = binary_search(b, nb, a[ma]);

        c[ma + mb] = a[ma];

#pragma omp task
        p_merge(c, a, b, ma, mb);
#pragma omp task
        p_merge(c + ma + mb + 1, a + ma + 1, b + mb, na - (ma + 1), nb - mb);
#pragma omp taskwait
    }
}

void p_merge_sort(int *b, int *a, int n)
{
    if (n == 1)
    {
        b[0] = a[0];
    }
    else
    {
        int *c = malloc(n * sizeof(int));

#pragma omp task
        p_merge_sort(c, a, n / 2);
#pragma omp task
        p_merge_sort(c + n / 2, a + n / 2, n - (n / 2));
#pragma omp taskwait
        p_merge(b, c, c + (n / 2), n / 2, n - (n / 2));

        free(c);
    }
}

void *setup()
{
    mem_t *mem = malloc(sizeof(mem_t));
    int *arr1 = malloc(ARRAY_SIZE * sizeof(*arr1));
    int *arr2 = malloc(ARRAY_SIZE * sizeof(*arr2));

    mem->a = arr1;
    mem->b = arr2;

    srand(7);

    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        arr1[i] = rand();
    }

    return mem;
}

int run(void *args)
{
    mem_t *mem = args;

    p_merge_sort(mem->b, mem->a, ARRAY_SIZE);

    return 0;
}

int main()
{
    mem_t *mem = (mem_t *)setup();

    // printf("[");
    // for (int i = 0; i < ARRAY_SIZE; i ++) {
    //     printf("%d, ", mem->a[i]);
    // }
    // printf("]\n");

    double start_time = omp_get_wtime();

    int ret = run(mem);

    double time = omp_get_wtime() - start_time;
    int numTries = 0;
    FILE *file = fopen("Ex03.csv", "a");
    while (fprintf(file, "parallel; %s; %f\n", getenv("OMP_NUM_THREADS"), time) < 1 && numTries < 10)
    {
        usleep(10000);
        numTries++;
        fseek(file, 0, SEEK_END);
    }
    fclose(file);

    // printf("[");
    // for (int i = 0; i < ARRAY_SIZE; i ++) {
    //     printf("%d, ", mem->b[i]);
    // }
    // printf("]\n");

    return ret;
}
