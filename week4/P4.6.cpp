#include <stdio.h>
#include <omp.h>

#define ARRAY_SIZE 10  

int main() {
    int array[ARRAY_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}; 
    int squared[ARRAY_SIZE];

    #pragma omp parallel for
    for (int i = 0; i < ARRAY_SIZE; i++) {
        squared[i] = array[i] * array[i];  
    }
    
    printf("Original Array:\n");
    for (int i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", array[i]);
    }
    printf("\n\nSquared Array:\n");
    for (int i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", squared[i]);
    }
   
    return 0;
}

