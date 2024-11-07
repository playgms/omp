#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int n, i;
    printf("Number of terms: ");
    scanf("%d", &n);  
    if (n < 2) {
        printf("Please enter a number greater than 1.\n");
        return 1;
    }

    int* a = (int*)malloc(n * sizeof(int));
    a[0] = 0;
    a[1] = 1;

    double start_time = omp_get_wtime();

    omp_set_num_threads(2);
    #pragma omp parallel
    {
        // Use one thread to compute Fibonacci numbers
        #pragma omp single
        {
            printf("Thread ID computing Fibonacci numbers: %d\n", omp_get_thread_num());
            for (i = 2; i < n; i++) {
                a[i] = a[i - 2] + a[i - 1];
            }
        }

        // Use another thread to display Fibonacci numbers
        #pragma omp single
        {
            printf("Thread ID displaying Fibonacci numbers: %d\n", omp_get_thread_num());
            printf("Fibonacci numbers: ");
            for (i = 0; i < n; i++) {
                printf("%d ", a[i]);
            }
            printf("\n");
        }
    }

    double end_time = omp_get_wtime();
    printf("Time Taken: %lf ms\n", (end_time - start_time) * 1000);

    free(a);  // Free allocated memory
    return 0;
}
