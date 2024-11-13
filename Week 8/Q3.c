#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 4  // Size of the matrix (system of equations)
#define ROOT 0

int main(int argc, char *argv[]) {
    int rank, size;
    double A[N][N], b[N], x[N];
    double local_A[N][N], local_b[N];
    int rows_per_process, start_row, end_row;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    rows_per_process = N / size;
    start_row = rank * rows_per_process;
    end_row = (rank + 1) * rows_per_process;

    // Root process initializes the matrix A and vector b
    if (rank == ROOT) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                A[i][j] = rand() % 10 + 1;  // Random values between 1 and 10
            }
            b[i] = rand() % 10 + 1;
        }
    }

    // Scatter the matrix A and vector b among all processes
    MPI_Scatter(A, rows_per_process * N, MPI_DOUBLE, local_A, rows_per_process * N, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);
    MPI_Scatter(b, rows_per_process, MPI_DOUBLE, local_b, rows_per_process, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);

    // Gaussian elimination (forward elimination)
    for (int k = 0; k < N; k++) {
        // Broadcast the pivot row to all processes
        MPI_Bcast(A[k], N, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);
        MPI_Bcast(&b[k], 1, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);

        // Perform the elimination step for each process's rows
        for (int i = start_row; i < end_row; i++) {
            if (i > k) {
                double factor = local_A[i][k] / A[k][k];
                for (int j = k; j < N; j++) {
                    local_A[i][j] -= factor * A[k][j];
                }
                local_b[i] -= factor * b[k];
            }
        }

        // Gather the modified matrix A and vector b to the root process
        MPI_Gather(local_A, rows_per_process * N, MPI_DOUBLE, A, rows_per_process * N, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);
        MPI_Gather(local_b, rows_per_process, MPI_DOUBLE, b, rows_per_process, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);
    }

    // Back substitution (root process only)
    if (rank == ROOT) {
        for (int i = N - 1; i >= 0; i--) {
            x[i] = b[i];
            for (int j = i + 1; j < N; j++) {
                x[i] -= A[i][j] * x[j];
            }
            x[i] /= A[i][i];
        }

        // Output the solution
        printf("Solution vector x:\n");
        for (int i = 0; i < N; i++) {
            printf("x[%d] = %f\n", i, x[i]);
        }
    }

    MPI_Finalize();
    return 0;
}
