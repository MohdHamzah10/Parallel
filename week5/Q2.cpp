#include <stdio.h>
#include <omp.h>
#include <time.h>

#define SIZE 1000

void sequential_average(int arr[], int size, double *avg) {
    long long total_sum = 0;
    int i;
    for (i = 0; i < size; i++) {
        total_sum += arr[i];
    }
    *avg = (double)total_sum / size;
}

void parallel_average(int arr[], int size, double *avg) {
    long long total_sum = 0;

    #pragma omp parallel
    {
        long long partial_sum = 0; 
        int i;

        // Calculate the partial sum
        #pragma omp for
        for (i = 0; i < size; i++) {
            partial_sum += arr[i];
        }

        // Combine the partial sums into the total sum
        #pragma omp atomic
        total_sum += partial_sum;
    }

    *avg = (double)total_sum / size;
}

int main() {
    int i,arr[SIZE];
    double avg_seq, avg_par;

    for ( i = 0; i < SIZE; i++) {
        arr[i] = i + 1; 
    }

    clock_t start_seq = clock();
    sequential_average(arr, SIZE, &avg_seq);
    clock_t end_seq = clock();
    double time_seq = (double)(end_seq - start_seq) / CLOCKS_PER_SEC;

    clock_t start_par = clock();
    parallel_average(arr, SIZE, &avg_par);
    clock_t end_par = clock();
    double time_par = (double)(end_par - start_par) / CLOCKS_PER_SEC;

    printf("Sequential Average: %.2f, Time taken: %.6f seconds\n", avg_seq, time_seq);
    printf("Parallel Average: %.2f, Time taken: %.6f seconds\n", avg_par, time_par);

    return 0;
}

