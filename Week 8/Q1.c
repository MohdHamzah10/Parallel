#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 16
#define ROOT 0

int main(int argc, char *argv[]) {
    int rank, size;
    int *arr = NULL;
    int *subArr = NULL;
    int *result = NULL;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int localN = N / size;

    if (rank == ROOT) {
        arr = (int *)malloc(N * sizeof(int));
        result = (int *)malloc(N * sizeof(int));
        for (int i = 0; i < N; i++) {
            arr[i] = i + 1;
        }
    }

    subArr = (int *)malloc(localN * sizeof(int));

    MPI_Scatter(arr, localN, MPI_INT, subArr, localN, MPI_INT, ROOT, MPI_COMM_WORLD);

    for (int i = 0; i < localN; i++) {
        subArr[i] = subArr[i] * subArr[i];
    }

    MPI_Gather(subArr, localN, MPI_INT, result, localN, MPI_INT, ROOT, MPI_COMM_WORLD);

    if (rank == ROOT) {
        printf("Processed array (squared elements):\n");
        for (int i = 0; i < N; i++) {
            printf("%d ", result[i]);
        }
        printf("\n");

        free(arr);
        free(result);
    }

    free(subArr);

    MPI_Finalize();
    return 0;
}
