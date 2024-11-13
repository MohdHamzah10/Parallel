#include <stdio.h>
#include <omp.h>

int main() {
    int number;
    long long result = 1;

    printf("Enter a positive integer: ");
    scanf("%d", &number);

    if (number < 0) {
        printf("Factorial is not defined for negative numbers.\n");
    } else {
        #pragma omp parallel for reduction(* : result)
        for (int i = 1; i <= number; i++) {
            result *= i;
        }

        printf("Factorial of %d is %lld\n", number, result);
    }

    return 0;
}

