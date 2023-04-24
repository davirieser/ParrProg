
#include <stdlib.h>
#include <stdio.h>
#include <bool.h>

#define ARRAY_SIZE 100000000

#define DO_PARALLEL false

typedef struct mem
{
    int *a;
    int *b;
} mem_t;

// Source: https://www.geeksforgeeks.org/merge-sort/

void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    /* create temp arrays */
    int L[n1], R[n2];

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    /* Copy the remaining elements of L[], if there
    are any */
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there
    are any */
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}

/* l is for left index and r is right index of the
sub-array of arr to be sorted */
void merge_sort(int arr[], int l, int r)
{
    if (l < r)
    {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l + (r - l) / 2;

        // Sort first and second halves
        merge_sort(arr, l, m);
        merge_sort(arr, m + 1, r);

        merge(arr, l, m, r);
    }
}

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

    // merge_sort(mem->b, 0, ARRAY_SIZE);
    if (DO_PARALLEL)
    {
        p_merge_sort(mem->b, mem->a, ARRAY_SIZE);
    }
    else
    {
        merge_sort(mem->a, 0, ARRAY_SIZE);
    }

    return 0;
}

int main()
{
    mem_t *mem = (mem_t *)setup();

    /*
    printf("[");
    for (int i = 0; i < ARRAY_SIZE; i ++) {
        printf("%d, ", mem->a[i]);
    }
    printf("]\n");
    */

    double start_time = omp_get_wtime();

    int ret = run(mem);

    double time = omp_get_wtime() - start_time;
    FILE *file = fopen("data.csv", "a");
    while (fprintf(file, "%s; %s; %f\n", DO_PARALLEL ? "parallel" : "serial", getenv("OMP_NUM_THREADS"), time) < 1 && numTries < 10)
    {
        usleep(10000);
        numTries++;
        fseek(file, 0, SEEK_END);
    }
    fclose(file);
    /*
    printf("[");
    for (int i = 0; i < ARRAY_SIZE; i ++) {
        printf("%d, ", mem->b[i]);
    }
    printf("]\n");
    */

    return ret;
}
