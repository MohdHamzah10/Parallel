#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 1024  // Array size must be a power of 2
#define MAX_VAL 100000

void bitonicMerge(int *arr, int low, int cnt, int dir) {
    if (cnt > 1) {
        int k = cnt / 2;
        #pragma omp parallel for
        for (int i = low; i < low + k; i++) {
            if ((arr[i] > arr[i + k]) == dir) {
                int temp = arr[i];
                arr[i] = arr[i + k];
                arr[i + k] = temp;
            }
        }
        bitonicMerge(arr, low, k, dir);
        bitonicMerge(arr, low + k, k, dir);
    }
}

void bitonicSort(int *arr, int low, int cnt, int dir) {
    if (cnt > 1) {
        int k = cnt / 2;
        #pragma omp parallel sections
        {
            #pragma omp section
            bitonicSort(arr, low, k, 1);  // Sorting in ascending order
            #pragma omp section
            bitonicSort(arr, low + k, k, 0);  // Sorting in descending order
        }
        bitonicMerge(arr, low, cnt, dir);
}

int isSorted(int *arr, int n) {
    for (int i = 1; i < n; i++) {
        if (arr[i - 1] > arr[i]) return 0;
    }
    return 1;
}

void sequentialBitonicSort(int *arr, int low, int cnt, int dir) {
    if (cnt > 1) {
        int k = cnt / 2;
        sequentialBitonicSort(arr, low, k, 1); // Sorting in ascending order
        sequentialBitonicSort(arr, low + k, k, 0); // Sorting in descending order
        bitonicMerge(arr, low, cnt, dir);
}

int main() {
    int *arr = malloc(N * sizeof(int));
    int *arr_copy = malloc(N * sizeof(int));

    // Initialize array with random integers
    for (int i = 0; i < N; i++) {
        arr[i] = rand() % MAX_VAL;
        arr_copy[i] = arr[i]; // Copy for sequential sorting
    }

    // Sequential Bitonic Sort timing
    double start = omp_get_wtime();
    sequentialBitonicSort(arr_copy, 0, N, 1);
    double end = omp_get_wtime();
    printf("Sequential BitonicSort time: %f seconds\n", end - start);

    if (isSorted(arr_copy, N)) {
        printf("Sequential BitonicSort successful.\n");
    } else {
        printf("Sequential BitonicSort failed.\n");
    }

    // Parallel Bitonic Sort timing
    start = omp_get_wtime();
    bitonicSort(arr, 0, N, 1);
    end = omp_get_wtime();
    printf("Parallel BitonicSort time: %f seconds\n", end - start);

    if (isSorted(arr, N)) {
        printf("Parallel BitonicSort successful.\n");
    } else {
        printf("Parallel BitonicSort failed.\n");
    }

    // Free allocated memory
    free(arr);
    free(arr_copy);

    return 0;
}
