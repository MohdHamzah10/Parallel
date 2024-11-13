#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define ARRAY_SIZE 1000

int main() {
    int array[ARRAY_SIZE];
   long long sum = 0;

    srand(0);

    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = rand(); 
    }

    #pragma omp parallel
    {
        long long local_sum = 0;

        #pragma omp for
        for (int i = 0; i < ARRAY_SIZE; i++) {
            local_sum+=array[i];
            }


        #pragma omp critical
        {
        	sum+=local_sum;	
        }
	}
	  printf("Sum of elements of the array: %lld\n",sum);
  
    return 0;
}

