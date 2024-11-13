#include <stdio.h>
#include <omp.h>

#define N 4  // Use a smaller size for easier readability in output

void print_matrix(double matrix[N][N], const char *name) {
    printf("Matrix %s:\n", name);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%6.0f ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void matrix_multiply(double A[N][N], double B[N][N], double C[N][N]) {
    int i, j, k;

    // Parallelize outer loops with OpenMP
    #pragma omp parallel for private(i, j, k) shared(A, B, C)
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            C[i][j] = 0.0;
            for (k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    double A[N][N], B[N][N], C[N][N];
    int i, j;

    // Initialize matrices A and B with some values
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            A[i][j] = i + j;
            B[i][j] = i - j;
            C[i][j] = 0.0;  // Initialize result matrix C to zero
        }
    }

    // Print matrices A and B before multiplication
    print_matrix(A, "A");
    print_matrix(B, "B");

    // Perform matrix multiplication
    matrix_multiply(A, B, C);

    // Print resulting matrix C
    print_matrix(C, "C (Result)");

    return 0;
}
