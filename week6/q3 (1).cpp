#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Define the size of the workload
#define WORKLOAD_SIZE 1000000

// Amdahl's Law function to calculate theoretical speedup
double amdahls_law(double p, int num_threads) {
    return 1.0 / ((1.0 - p) + (p / num_threads));
}

int main() {
    // Define the parallelizable fraction (70%) and sequential fraction (30%)
    const double parallel_fraction = 0.7;
    const double sequential_fraction = 1.0 - parallel_fraction;

    double *data = (double *)malloc(WORKLOAD_SIZE * sizeof(double));
    double parallel_result = 0.0;
    double sequential_result = 0.0;

    // Initialize data array with some values
    for (int i = 0; i < WORKLOAD_SIZE; i++) {
        data[i] = 1.0;  // Set each element to 1 for simplicity
    }

    // Record start time for sequential work
    double start_time = omp_get_wtime();

    // (30% of the workload)
    for (int i = 0; i < WORKLOAD_SIZE * sequential_fraction; i++) {
        sequential_result += data[i] * 0.5;  // Simple computation
    }

    //  (70% of the workload) with OpenMP
    #pragma omp parallel for reduction(+:parallel_result)
    for (int i = WORKLOAD_SIZE * sequential_fraction; i < WORKLOAD_SIZE; i++) {
        parallel_result += data[i] * 0.5;  // Simple computation
    }

    // Record end time and calculate actual speedup
    double end_time = omp_get_wtime();
    double elapsed_time = end_time - start_time;
    printf("Elapsed Time (Mixed Workload): %f seconds\n", elapsed_time);

    // Total result is the sum of the parallel and sequential parts
    double total_result = sequential_result + parallel_result;
    printf("Total Result: %f\n", total_result);

    // Number of threads used in the parallel region
    int num_threads;
    #pragma omp parallel
    {
        #pragma omp single
        num_threads = omp_get_num_threads();
    }
    printf("Number of Threads: %d\n", num_threads);

    // Calculate theoretical speedup using Amdahl's Law
    double theoretical_speedup = amdahls_law(parallel_fraction, num_threads);
    printf("Theoretical Speedup (Amdahl's Law): %f\n", theoretical_speedup);

    // Calculate actual speedup by comparing with a single-thread run
    start_time = omp_get_wtime();
    double single_thread_result = 0.0;

    // Run the entire workload on a single thread for comparison
    for (int i = 0; i < WORKLOAD_SIZE; i++) {
        single_thread_result += data[i] * 0.5;
    }
    end_time = omp_get_wtime();
    double single_thread_time = end_time - start_time;
    double actual_speedup = single_thread_time / elapsed_time;

    printf("Single-thread Time: %f seconds\n", single_thread_time);
    printf("Actual Speedup: %f\n", actual_speedup);

    // Clean up
    free(data);
    return 0;
}

