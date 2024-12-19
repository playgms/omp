#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int rank, numproc;
    int sum = 0;
    int total_sum = 0;

    MPI_Init(&argc, &argv); // Initialize the MPI environment
    MPI_Comm_size(MPI_COMM_WORLD, &numproc); // Get the number of processes
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);   // Get the rank of the process

    // Seed the random number generator with the rank
    srand(rank + 1); // Using rank + 1 to ensure different seeds for different ranks
    sum = rand() % 100; // Generate a random number between 0 and 99

    printf("Robot %d picked %d mangoes.\n", rank, sum);

    // Perform a reduction to sum the mangoes across all processes
    MPI_Reduce(&sum, &total_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Rank 0 process prints the total sum
    if (rank == 0) {
        printf("Total Mangoes picked by %d Robots = %d\n", numproc, total_sum);
    }

    MPI_Finalize(); // Finalize the MPI environment
    return 0;
}

//mpicc -o mango_picker mango_picker.c
//mpirun -np 4 ./mango_picker

