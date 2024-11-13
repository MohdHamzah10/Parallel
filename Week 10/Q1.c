#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Partition function to rearrange elements around the pivot
int partition(int *arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    int temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    return i + 1;
}

// Recursive Quick Sort function with OpenMP parallelization
void parallelQuickSort(int *arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        // Parallelize recursive calls for left and right partitions
        #pragma omp task shared(arr) if(high - low > 1000) // Use tasks for large subarrays
        parallelQuickSort(arr, low, pi - 1);

        #pragma omp task shared(arr) if(high - low > 1000)
        parallelQuickSort(arr, pi + 1, high);
    }
}

// Function to launch parallel Quick Sort
void quickSort(int *arr, int low, int high) {
    #pragma omp parallel
    {
        #pragma omp single
        parallelQuickSort(arr, low, high);
    }
}

// Sequential Quick Sort for comparison
void sequentialQuickSort(int *arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        sequentialQuickSort(arr, low, pi - 1);
        sequentialQuickSort(arr, pi + 1, high);
    }
}

// Helper function to check if array is sorted
int isSorted(int *arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) return 0;
    }
    return 1;
}

int main() {
    int n = 100000;
    int *arr = malloc(n * sizeof(int));
    int *arr_copy = malloc(n * sizeof(int));

    // Initialize array with random integers
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 100000;
        arr_copy[i] = arr[i]; // Copy for sequential sorting
    }

    // Sequential Quick Sort timing
    double start = omp_get_wtime();
    sequentialQuickSort(arr_copy, 0, n - 1);
    double end = omp_get_wtime();
    printf("Sequential QuickSort time: %f seconds\n", end - start);

    // Check if sequential sort was successful
    if (isSorted(arr_copy, n)) {
        printf("Sequential QuickSort successful.\n");
    } else {
        printf("Sequential QuickSort failed.\n");
    }

    // Parallel Quick Sort timing
    start = omp_get_wtime();
    quickSort(arr, 0, n - 1);
    end = omp_get_wtime();
    printf("Parallel QuickSort time: %f seconds\n", end - start);

    // Check if parallel sort was successful
    if (isSorted(arr, n)) {
        printf("Parallel QuickSort successful.\n");
    } else {
        printf("Parallel QuickSort failed.\n");
    }

    // Free allocated memory
    free(arr);
    free(arr_copy);

    return 0;
}
