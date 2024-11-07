#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int main() {
    int m, n;
    printf("Enter the size of square matrix: ");
    scanf("%d", &n);
    printf("Enter the size of vector: ");
    scanf("%d", &m);

    if (m != n) {
        printf("Multiplication is not possible.\n");
        exit(0);
    }

    int i, j;
    int **arr = (int **)malloc(n * sizeof(int *));
    int *vec = (int *)malloc(n * sizeof(int));
    int *res = (int *)malloc(n * sizeof(int));

    // Initialize random number generation
    srand(time(NULL));

    // Allocate and initialize the matrix and vector
    #pragma omp parallel private(j)
    {
        #pragma omp for
        for (i = 0; i < n; i++) {
            arr[i] = (int *)malloc(n * sizeof(int));
            vec[i] = rand() % 100; // Vector values
            for (j = 0; j < n; j++)
                arr[i][j] = rand() % 100; // Matrix values
        }
    }

    // Perform matrix-vector multiplication
    #pragma omp parallel for private(j)
    for (i = 0; i < n; i++) {
        res[i] = 0;
        for (j = 0; j < n; j++)
            res[i] += arr[i][j] * vec[j];
    }

    // Print the result
    printf("Matrix * Vector = Resultant Vector\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++)
            printf("%3d ", arr[i][j]);
        if (i == n / 2)
            printf("  *  %3d  = %6d\n", vec[i], res[i]);
        else
            printf("     %3d    %6d\n", vec[i], res[i]);
    }

    // Free allocated memory
    for (i = 0; i < n; i++)
        free(arr[i]);
    free(arr);
    free(vec);
    free(res);

    return 0; // Return 0 for successful completion
}


// gcc -fopenmp p1.c
