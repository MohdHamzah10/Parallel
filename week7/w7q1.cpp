#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define MAX_VALUE 100 // Maximum value in the array for simplicity

void count_frequencies_parallel(int *arr, int *freq, int n, int num_threads) {
    // Initialize OpenMP parallel region
    #pragma omp parallel for num_threads(num_threads)
    for (int i = 0; i < n; i++) {
        // Ensure thread-safe updates to the frequency array using atomic operation
        #pragma omp atomic
        freq[arr[i]]++;
    }
}

int main() {
    int n = 1000000;  // Array size
    int *arr = (int *)malloc(n * sizeof(int));
    int *freq = (int *)calloc(MAX_VALUE, sizeof(int));
    int num_threads = 4;  // Number of threads (change for different runs)
    
    // Initialize random seed and array
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % MAX_VALUE;
    }
    
    // Count frequencies in parallel
    clock_t start_time = clock();
    count_frequencies_parallel(arr, freq, n, num_threads);
    clock_t end_time = clock();
    
    printf("Parallel execution time (Threads: %d): %lf seconds\n", num_threads, (double)(end_time - start_time) / CLOCKS_PER_SEC);
    
    free(arr);
    free(freq);
    return 0;
}

