#include <stdio.h>
#include <omp.h>

#define ARRAY_SIZE 10  // Size of the arrays

int main() {
    int vector_a[ARRAY_SIZE] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    int vector_b[ARRAY_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int result[ARRAY_SIZE];


    #pragma omp parallel for
    for (int i = 0; i < ARRAY_SIZE; i++) {
        result[i] = vector_a[i] - vector_b[i]; 
    }

    printf("Vector A:\n");
    for (int i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", vector_a[i]);
    }
    printf("\n\nVector B:\n");
    for (int i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", vector_b[i]);
    }
    printf("\n\nResult of Subtraction (Vector A - Vector B):\n");
    for (int i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", result[i]);
    }

    return 0;
}

