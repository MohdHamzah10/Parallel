#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

// Comparator function for qsort
int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

// Function to merge two sorted arrays
void merge(int *arr1, int size1, int *arr2, int size2, int *result) {
    int i = 0, j = 0, k = 0;
    while (i < size1 && j < size2) {
        if (arr1[i] < arr2[j]) result[k++] = arr1[i++];
        else result[k++] = arr2[j++];
    }
    while (i < size1) result[k++] = arr1[i++];
    while (j < size2) result[k++] = arr2[j++];
}

int main(int argc, char *argv[]) {
    int rank, size, n = 16;
    int *data = NULL;

    // Initialize MPI environment
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Root process initializes the array
    if (rank == 0) {
        data = (int *)malloc(n * sizeof(int));
        for (int i = 0; i < n; i++) data[i] = rand() % 100;
    }

    // Divide data equally among processes
    int chunk_size = n / size;
    int *sub_array = (int *)malloc(chunk_size * sizeof(int));

    // Scatter data to all processes
    MPI_Scatter(data, chunk_size, MPI_INT, sub_array, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Each process sorts its sub-array
    qsort(sub_array, chunk_size, sizeof(int), compare);

    // Progressive merging
    int step = 1;
    while (step < size) {
        if (rank % (2 * step) == 0) {
            if (rank + step < size) {
                int recv_size = chunk_size * step;
                int *recv_array = (int *)malloc(recv_size * sizeof(int));
                MPI_Recv(recv_array, recv_size, MPI_INT, rank + step, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                // Merge received array with current array
                int *temp_array = (int *)malloc((chunk_size + recv_size) * sizeof(int));
                merge(sub_array, chunk_size, recv_array, recv_size, temp_array);
                free(sub_array);
                free(recv_array);
                sub_array = temp_array;
                chunk_size += recv_size;
            }
        } else {
            int near_rank = rank - step;
            MPI_Send(sub_array, chunk_size, MPI_INT, near_rank, 0, MPI_COMM_WORLD);
            break;
        }
        step *= 2;
    }

    // Root process prints the sorted array
    if (rank == 0) {
        printf("Sorted Array: ");
        for (int i = 0; i < n; i++) printf("%d ", sub_array[i]);
        printf("\n");
        free(data);
    }

    free(sub_array);
    MPI_Finalize();
    return 0;
}
