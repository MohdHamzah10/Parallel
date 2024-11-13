
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void initialize_array(int *array, int n) {
    // Declare loop variable beforehand
    int i;

    #pragma omp parallel firstprivate(n) private(i)
    {
        // Get the number of threads and calculate chunk size
        int num_threads = omp_get_num_threads();
        int chunk_size = n / num_threads;

        // Calculate the start and end values for each thread
        int thread_id = omp_get_thread_num();
        int start_value = thread_id * chunk_size + 1;
        int last_value = start_value + chunk_size - 1;

        // Ensure that the last thread handles any remaining elements
        if (thread_id == num_threads - 1) {
            last_value = n; // The last thread should go up to n
        }

        // Initialize the array chunk
        for (i = start_value; i <= last_value; i++) {
            array[i - 1] = i; // Fill the array (0-based index)
        }

        // Capture the last initialized value (if needed)
        #pragma omp critical
        {
            printf("Thread %d initialized up to %d\n", thread_id, last_value);
        }
    } // End of parallel region
}

int main() {
    int n = 100,i; // Size of the array
    int *array = (int *)malloc(n * sizeof(int));

    initialize_array(array, n);

    // Print the array (optional)
    for ( i = 0; i < n; i++) {
        printf("%d ", array[i]);
    }
}

