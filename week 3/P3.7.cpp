#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NUM_POINTS 1000000  

int main() {
    int count = 0;

    #pragma omp parallel
    {
        int local_count = 0; 
        unsigned int seed = omp_get_thread_num(); 

        #pragma omp for
        for (int i = 0; i < NUM_POINTS; i++) {
            
            double x = (double)rand() / RAND_MAX; 
            double y = (double)rand() / RAND_MAX; 

           
            if (x * x + y * y <= 1.0) {
                local_count++;
            }
        }

        #pragma omp atomic
        count += local_count; 
    }

   
    double pi = 4.0 * count / NUM_POINTS;
    printf("Estimated value of pi: %f\n", pi);

    return 0;
}

