#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void double_array(int *array, int n) {
    int num_threads, chunk_size,i;

    #pragma omp parallel firstprivate(n, chunk_size) private(num_threads)
    {
        // Get the number of threads and calculate chunk size
        num_threads = omp_get_num_threads();
        chunk_size = n / num_threads;

        // Calculate the starting index for each thread
        int thread_id = omp_get_thread_num();
        int start_index = thread_id * chunk_size;
        int end_index = start_index + chunk_size - 1;

        // Handle the last thread's case for odd sizes
        if (thread_id == num_threads - 1) {
            end_index = n - 1; // Last thread processes remaining elements
        }

        // Double the values in the assigned chunk
        for ( i = start_index; i <= end_index; i++) {
            array[i] *= 2; // Double the value
        }

        // Capture the last processed index for each thread
        int last_processed_index = end_index;

        //  print out the last processed index in a critical section
        #pragma omp critical
        {
            printf("Thread %d processed up to index %d\n", thread_id, last_processed_index);
        }
    }
}

int main() {
    int n = 100,i; // Size of the array
    int *array = (int *)malloc(n * sizeof(int));

    // Initialize the array with some values
    for ( i = 0; i < n; i++) {
        array[i] = i + 1; // Fill with values 1 to n
    }

    double_array(array, n);

    for ( i = 0; i < n; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    free(array);
    return 0;
}

