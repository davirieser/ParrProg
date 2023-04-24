
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define ARRAY_SIZE 100000000

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
    int * L = malloc(n1 * sizeof(int)), * R = malloc(n2 * sizeof(int));

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

    free(L);
    free(R);
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

    merge_sort(mem->b, 0, ARRAY_SIZE);

    return 0;
}

int main()
{
    mem_t *mem = (mem_t *)setup();

    printf("[");
    for (int i = 0; i < ARRAY_SIZE; i ++) {
        printf("%d, ", mem->a[i]);
    }
    printf("]\n");

    int ret = run(mem);
    
    printf("[");
    for (int i = 0; i < ARRAY_SIZE; i ++) {
        printf("%d, ", mem->b[i]);
    }
    printf("]\n");

    return ret;
}
