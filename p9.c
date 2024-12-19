#include <stdio.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    int rank, size, next, prev, inmsg;
    MPI_Request reqs[2];
    MPI_Status stats[2];

    MPI_Init(&argc, &argv);                   // Initialize MPI
    MPI_Comm_size(MPI_COMM_WORLD, &size);     // Get the number of processes
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);     // Get the rank of this process

    // Determine neighbors
    next = (rank + 1) % size;                 // Next process in ring
    prev = (rank - 1 + size) % size;          // Previous process in ring

    // Blocking Send/Receive
    if (rank == 0) {
        printf("Blocking Send/Receive:\n");
    }
    MPI_Barrier(MPI_COMM_WORLD); // Synchronize before starting

    if (rank % 2 == 0) {
        MPI_Send(&rank, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
        MPI_Recv(&inmsg, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else {
        MPI_Recv(&inmsg, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(&rank, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
    }

    printf("Rank %d received %d (Blocking)\n", rank, inmsg);

    // Non-blocking Send/Receive
    if (rank == 0) {
        printf("\nNon-blocking Send/Receive:\n");
    }
    MPI_Barrier(MPI_COMM_WORLD); // Synchronize before starting

    MPI_Irecv(&inmsg, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, &reqs[0]);
    MPI_Isend(&rank, 1, MPI_INT, next, 0, MPI_COMM_WORLD, &reqs[1]);

    MPI_Waitall(2, reqs, stats); // Wait for both non-blocking operations to complete

    printf("Rank %d received %d (Non-blocking)\n", rank, inmsg);

    MPI_Finalize(); // Finalize MPI
    return 0;
}

//mpicc p9.c
//mpirun -np 4 ./a.out
