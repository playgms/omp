#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int main() {
    int n;
    printf("Enter the dimension of square matrices: ");
    scanf("%d", &n); // Changed to scanf for compatibility

    int i = 0, j = 0, k = 0;
    int** arr1 = (int**)malloc(n * sizeof(int*));
    int** arr2 = (int**)malloc(n * sizeof(int*));
    int** res = (int**)malloc(n * sizeof(int*));

    // Set number of threads for OpenMP
    omp_set_num_threads(64);

    // Allocate and initialize matrices
    #pragma omp parallel private(j)
    {
        #pragma omp for
        for (i = 0; i < n; i++) {
            arr1[i] = (int*)malloc(n * sizeof(int));
            arr2[i] = (int*)malloc(n * sizeof(int));
            res[i] = (int*)malloc(n * sizeof(int));
            unsigned int seed = i + 1; // Thread-safe random seeding
            for (j = 0; j < n; j++) {
                arr1[i][j] = rand_r(&seed) % 100;
                arr2[i][j] = rand_r(&seed) % 100;
            }
        }
    }

    clock_t st, et;

    // Parallel matrix multiplication
    st = clock();
    #pragma omp parallel private(j, k)
    {
        #pragma omp for
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                res[i][j] = 0;
                for (k = 0; k < n; k++) {
                    res[i][j] += arr1[i][k] * arr2[k][j];
                }
            }
        }
    }
    et = clock();
    printf("Time taken by parallel algorithm: %lf seconds\n", (double)(et - st) / CLOCKS_PER_SEC);

    // Sequential matrix multiplication
    st = clock();
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            res[i][j] = 0;
            for (k = 0; k < n; k++) {
                res[i][j] += arr1[i][k] * arr2[k][j];
            }
        }
    }
    et = clock();
    printf("Time taken by sequential algorithm: %lf seconds\n", (double)(et - st) / CLOCKS_PER_SEC);

    // Free allocated memory
    for (i = 0; i < n; i++) {
        free(arr1[i]);
        free(arr2[i]);
        free(res[i]);
    }
    free(arr1);
    free(arr2);
    free(res);

    return 0;
}
