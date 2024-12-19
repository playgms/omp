#include <stdio.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    int rank, size, next, prev, inmsg;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Determine neighbors
    next = (rank + 1) % size;
    prev = (rank - 1 + size) % size;

    // Blocking Communication
    MPI_Barrier(MPI_COMM_WORLD); // Synchronize before starting
    if (rank % 2 == 0) {
        MPI_Send(&rank, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
        MPI_Recv(&inmsg, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else {
        MPI_Recv(&inmsg, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(&rank, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
    }

    // Ensure ordered output for Blocking
    for (int i = 0; i < size; i++) {
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank == i) {
            printf("Rank %d received %d (Blocking)\n", rank, inmsg);
        }
    }

    // Non-blocking Communication
    MPI_Request reqs[2];
    MPI_Barrier(MPI_COMM_WORLD); // Synchronize before starting
    MPI_Irecv(&inmsg, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, &reqs[0]);
    MPI_Isend(&rank, 1, MPI_INT, next, 0, MPI_COMM_WORLD, &reqs[1]);
    MPI_Waitall(2, reqs, MPI_STATUSES_IGNORE);

    // Ensure ordered output for Non-blocking
    for (int i = 0; i < size; i++) {
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank == i) {
            printf("Rank %d received %d (Non-blocking)\n", rank, inmsg);
        }
    }

    MPI_Finalize();
    return 0;
}
