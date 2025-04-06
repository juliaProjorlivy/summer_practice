#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <limits.h>
#include <sys/types.h>

typedef int arr_t;
#define MAX_VAL INT_MAX

enum dir_t
{
    DESC = 0,
    ASC = 1,
};

struct SortArgs
{
    arr_t *array;
    uint left;
    uint right;
    enum dir_t dir;
};

void compAndSwap(int *array, int i, int j, enum dir_t dir)
{
    if ((array[i] > array[j]) == dir)
    {
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

void *bitonicMerge(void *arg)
{
    struct SortArgs *args = (struct SortArgs *)arg;
    arr_t *array = args->array;
    arr_t left = args->left;
    arr_t right = args->right;
    enum dir_t dir = args->dir;

    int n = right - left + 1;
    if(n > 1)
    {
        int m = n / 2;
        for (int i = left; i < left + m; i++)
        {
            compAndSwap(array, i, i + m, dir);
        }

        pthread_t threads[2];
        struct SortArgs args1 = {array, left, left + m - 1, dir};
        struct SortArgs args2 = {array, left + m, right, dir};

        pthread_create(&threads[0], NULL, bitonicMerge, &args1);
        pthread_create(&threads[1], NULL, bitonicMerge, &args2);

        pthread_join(threads[0], NULL);
        pthread_join(threads[1], NULL);
    }
    return NULL;
}

void *bitonicSort(void *arg)
{
    struct SortArgs *args = (struct SortArgs*)arg;
    arr_t *array = args->array;
    arr_t left = args->left;
    arr_t right = args->right;
    enum dir_t dir = args->dir;

    int n = right - left + 1;
    if (n > 1)
    {
        int m = n / 2;

        pthread_t threads[2];
        struct SortArgs args1 = {array, left, left + m - 1, ASC};
        struct SortArgs args2 = {array, left + m, right, DESC};

        pthread_create(&threads[0], NULL, bitonicSort, &args1);
        pthread_create(&threads[1], NULL, bitonicSort, &args2);

        pthread_join(threads[0], NULL);
        pthread_join(threads[1], NULL);

        struct SortArgs mergeParams = {array, left, right, dir};
        bitonicMerge(&mergeParams);
    }
    return NULL;
}

void parallelBitonicSort(int *array, int size, enum dir_t dir)
{
    struct SortArgs args = {array, 0, size - 1, dir};
    bitonicSort(&args);
}

int isPowerOfTwo(uint x)
{
    return (x & (x - 1)) == 0;
}

uint getCloseToPowerTwo(uint x)
{
    uint power = (uint)log2((double)x) + 1;
    return 1 << power;
}

void fillLastBig(arr_t *arr, uint size, uint n)
{
    for(uint i = 1; i <= n; ++i)
    {
        arr[size-i] = MAX_VAL;
    }
}

int main()
{
    uint N = 0;
    if(scanf("%d", &N) == EOF)
    {
        printf("failed to scanf number of elements in array\n");
        return 1;
    }
    uint size = N;

    if(!isPowerOfTwo(N))
    {
        size = getCloseToPowerTwo(N);
    }
    arr_t *arr = (arr_t *)calloc(sizeof(arr_t), size);
    fillLastBig(arr, size, size - N);


    for(uint i = 0; i < N; i++)
    {
        if(scanf("%d", &arr[i]) == EOF)
        {
            printf("failed to scanf the array\n");
            return 1;
        }
    }

    printf("Input array: ");
    for (int i = 0; i < N; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    parallelBitonicSort(arr, size, ASC);

    printf("Sorted array: ");
    for (int i = 0; i < N; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    free(arr);

    return 0;
}

