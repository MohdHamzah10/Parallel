#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 1000  
#define MAX_ITER 1000
#define TOL 1e-6

void parallelGaussSeidel(double A[N][N], double b[N], double x[N]) {
    double x_new[N];

    #pragma omp parallel
    {
        for (int k = 0; k < MAX_ITER; k++) {
            #pragma omp barrier

            #pragma omp for
            for (int i = 0; i < N; i++) {
                double sum = b[i];
                for (int j = 0; j < N; j++) {
                    if (i != j) {
                        sum -= A[i][j] * x[j];
                    }
                }
                x_new[i] = sum / A[i][i];
            }

            #pragma omp barrier

            #pragma omp for
            for (int i = 0; i < N; i++) {
                x[i] = x_new[i];
            }

            if (converged(x, x_new)) {
                printf("Converged after %d iterations.\n", k + 1);
                break;
            }
        }
    }
}

int converged(double x[N], double x_new[N]) {
    double norm = 0.0;
    for (int i = 0; i < N; i++) {
        norm += (x_new[i] - x[i]) * (x_new[i] - x[i]);
    }
    return norm < TOL * TOL;
}

void initialize(double A[N][N], double b[N], double x[N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = (i == j) ? 2.0 : 1.0;
        }
        b[i] = 1.0;
        x[i] = 0.0;
    }
}

int main() {
    double A[N][N], b[N], x[N];

    initialize(A, b, x);

    double start_time = omp_get_wtime();
    parallelGaussSeidel(A, b, x);
    double end_time = omp_get_wtime();

    printf("Time taken for parallel Gauss-Seidel: %f seconds\n", end_time - start_time);

    return 0;
}
