#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int main() {
    int num, i;
    printf("Enter the number of steps: ");
    scanf("%d", &num); // Use scanf instead of scanf_s

    clock_t st, et; //clock_t for better precision
    st = clock();
    double step = 1.0 / (double)num, pi = 0.0;

    omp_set_num_threads(omp_get_max_threads()); 
    #pragma omp parallel for reduction(+:pi)
    for (i = 0; i < num; i++) {
        double x = (i + 0.5) * step;
        double local_pi = (4.0 * step) / (1 + x * x);
        pi += local_pi;
    }
    
    et = clock();
    printf("Time Taken: %lf seconds\n", (double)(et - st) / CLOCKS_PER_SEC);
    printf("Value of Pi = %.16lf\n", pi);
    
    return 0; // Return 0 to indicate successful completion
}


gcc -fopenmp your_program.c -o your_program

./your_program

