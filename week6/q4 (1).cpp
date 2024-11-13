#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Function to count occurrences of a specific value sequentially
int count_occurrences_sequential(int *array, int size, int value) {
    int count = 0;
    for (int i = 0; i < size; i++) {
        if (array[i] == value) {
            count++;
        }
    }
    return count;
}

// Function to count occurrences of a specific value using OpenMP with critical section
int count_occurrences_parallel(int *array, int size, int value) {
    int count = 0;

    #pragma omp parallel for
    for (int i = 0; i < size; i++) {
        if (array[i] == value) {
            #pragma omp critical
            count++;
        }
    }
    return count;
}

int main() {
    int size = 1000000;  // Array size (can adjust for testing)
    int target_value = 7; // Value to count in the array

    // Allocate and initialize array with random values between 1 and 10
    int *array = (int *)malloc(size * sizeof(int));
    for (int i = 0; i < size; i++) {
        array[i] = rand() % 10 + 1;  // Values between 1 and 10
    }

    // Measure time for sequential counting
    double start_time = omp_get_wtime();
    int sequential_count = count_occurrences_sequential(array, size, target_value);
    double end_time = omp_get_wtime();
    double sequential_time = end_time - start_time;
    printf("Sequential Count: %d\n", sequential_count);
    printf("Sequential Execution Time: %f seconds\n", sequential_time);

    // Measure time for parallel counting
    start_time = omp_get_wtime();
    int parallel_count = count_occurrences_parallel(array, size, target_value);
    end_time = omp_get_wtime();
    double parallel_time = end_time - start_time;
    printf("Parallel Count: %d\n", parallel_count);
    printf("Parallel Execution Time: %f seconds\n", parallel_time);

    // Verify that the counts match
    if (sequential_count == parallel_count) {
        printf("Counts match.\n");
    } else {
        printf("Counts do NOT match.\n");
    }

    // Calculate speedup and efficiency
    int num_threads;
    #pragma omp parallel
    {
        #pragma omp single
        num_threads = omp_get_num_threads();
    }
    double speedup = sequential_time / parallel_time;
    double efficiency = speedup / num_threads;

    printf("Number of Threads: %d\n", num_threads);
    printf("Speedup: %f\n", speedup);
    printf("Efficiency: %f\n", efficiency);

    // Clean up
    free(array);
    return 0;
}

