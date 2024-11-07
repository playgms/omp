#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int main() {
    int r, i, ans = 0;
    printf("Enter number of sections: ");
    scanf("%d", &r);

    int **arr = (int **)malloc(r * sizeof(int *));
    int *size = (int *)malloc(r * sizeof(int));
    
    srand(time(NULL)); 

    #pragma omp parallel
    {
        // Temporary variable for each thread's sum
        int thread_sum = 0;
        
        #pragma omp for
        for (i = 0; i < r; i++) {
            int j;
            size[i] = rand() % 20; // Set size for each section
            arr[i] = (int *)malloc(size[i] * sizeof(int));
            
            for (j = 0; j < size[i]; j++) {
                arr[i][j] = rand() % 100; // Fill section with random values
                thread_sum += arr[i][j]; // Sum for this thread
            }
        }

        // Use a critical section to update the total sum
        #pragma omp atomic
        ans += thread_sum;
    }

    // Output the results
    for (i = 0; i < r; i++) {
        printf("Section - %2d ( %3d Items ) : ", i, size[i]);
        for (int j = 0; j < size[i]; j++)
            printf("%3d ", arr[i][j]);
        printf("\n");
    }
    
    printf("Total Amount : %d\n", ans);

    // Free allocated memory
    for (i = 0; i < r; i++)
        free(arr[i]);
    free(arr);
    free(size);

    return 0; // Return 0 for successful completion
}
