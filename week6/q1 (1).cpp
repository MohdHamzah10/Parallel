#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int n = 1000000;  // Array size
    double *array = (double *)malloc(n * sizeof(double));  // Dynamically allocate the array
    double sum = 0.0;

    // Initialize the array with some values, for example, setting each element to 1.0
    for (int i = 0; i < n; i++) {
        array[i] = 1.0;
    }

    // Measure start time
    double start_time = omp_get_wtime();

    // Parallel sum with OpenMP using reduction to prevent data races
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < n; i++) {
        sum += array[i];
    }

    // Measure end time
    double end_time = omp_get_wtime();

    printf("Sum: %f\n", sum);
    printf("Execution Time: %f seconds\n", end_time - start_time);

    free(array);  // Free allocated memory
    return 0;
}

