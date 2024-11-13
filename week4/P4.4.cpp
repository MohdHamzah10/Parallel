#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define ARRAY_SIZE 1000

int main() {
    int array[ARRAY_SIZE];
    int max = 0;

    srand(0);

    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = rand(); 
    }

    #pragma omp parallel
    {
        int local_max = 0;

        #pragma omp for
        for (int i = 0; i < ARRAY_SIZE; i++) {
            if (array[i] > local_max) {
                local_max = array[i];
            }
        }

        #pragma omp critical
        {
            if (local_max > max) {
                max = local_max;
            }
        }
    }
	  printf("Maximum value in the array: %d\n", max);
  
    return 0;
}

