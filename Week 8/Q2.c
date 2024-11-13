#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 1000000  // Size of the dataset
#define ROOT 0  // Root process

int main(int argc, char *argv[]) {
    int rank, size;
    double *data = NULL;
    double local_sum = 0.0, global_sum = 0.0, local_count = 0, global_count = 0;
    int local_n;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Divide dataset among processes
    local_n = N / size;

    // Root process initializes the dataset
    if (rank == ROOT) {
        data = (double *)malloc(N * sizeof(double));
        // Initialize the dataset with some values (e.g., sequential numbers)
        for (int i = 0; i < N; i++) {
            data[i] = i + 1.0;
        }
    }

    // Allocate memory for the sub-array for each process
    double *sub_data = (double *)malloc(local_n * sizeof(double));

    // Scatter the data among all processes
    MPI_Scatter(data, local_n, MPI_DOUBLE, sub_data, local_n, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);

    // Each process computes the local sum and count
    for (int i = 0; i < local_n; i++) {
        local_sum += sub_data[i];
        local_count++;
    }

    // Gather the local sums and counts to the root process
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, ROOT, MPI_COMM_WORLD);
    MPI_Reduce(&local_count, &global_count, 1, MPI_DOUBLE, MPI_SUM, ROOT, MPI_COMM_WORLD);

    // Root process computes and prints the global average
    if (rank == ROOT) {
        double global_average = global_sum / global_count;
        printf("Global average: %f\n", global_average);

        // Free the allocated memory
        free(data);
    }

    // Free the sub-array for each process
    free(sub_data);

    MPI_Finalize();
    return 0;
}
