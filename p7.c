#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    int size, rank;

    MPI_Init(&argc, &argv); // Initialize the MPI environment
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Get the rank of the process
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Get the total number of processes

    float recvbuf, sendbuf[100];

    if (rank == 0) {
        int i;
        printf("Before Scatter: sendbuf of rank 0: ");
        for (i = 0; i < size; i++) {
            srand(i + 1); // Ensure unique seed for each process
            sendbuf[i] = (float)(rand() % 1000) / 10; // Generate random float values
            printf("%.1f ", sendbuf[i]);
        }
        printf("\nAfter Scatter:\n");
    }

    // Scatter the data from the root process to all other processes
    MPI_Scatter(sendbuf, 1, MPI_FLOAT, &recvbuf, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // Print the data received by each process
    printf("rank= %d Recvbuf: %.1f\n", rank, recvbuf);

    MPI_Finalize(); // Finalize the MPI environment
    return 0;
}


//mpicc -o scatter_example scatter_example.c
//mpirun -np 4 ./scatter_example
