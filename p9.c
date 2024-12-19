#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    int numtasks, rank, rc, sz, inmsg, next, prev;
    MPI_Status Stat;
    time_t st, et;

    MPI_Init(&argc, &argv);                   // Initialize MPI
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks); // Get the number of processes
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);     // Get the rank of the process

    // Ensure an even number of processes
    sz = (numtasks / 2) * 2;

    // Blocking send/receive
    st = clock();

    // Determine neighbors for blocking communication
    prev = (rank == 0) ? sz - 1 : rank - 1;
    next = (rank == sz - 1) ? 0 : rank + 1;

    if (rank % 2 == 0 && rank < sz) {
        rc = MPI_Send(&rank, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
        rc = MPI_Recv(&inmsg, 1, MPI_INT, prev, 1, MPI_COMM_WORLD, &Stat);
    } else if (rank % 2 == 1 && rank < sz) {
        rc = MPI_Recv(&inmsg, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, &Stat);
        rc = MPI_Send(&rank, 1, MPI_INT, next, 1, MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD); // Synchronize all processes
    et = clock();
    if (rank == 0)
        printf("Time taken by Blocking send/receive: %lf seconds\n", (double)(et - st) / CLOCKS_PER_SEC);

    MPI_Barrier(MPI_COMM_WORLD); // Synchronize before starting non-blocking communication

    // Non-blocking send/receive
    MPI_Request reqs[2];
    MPI_Status stats[2];
    st = clock();

    // Determine neighbors for non-blocking communication
    next = (rank == numtasks - 1) ? 0 : rank + 1;
    prev = (rank == 0) ? numtasks - 1 : rank - 1;

    MPI_Irecv(&inmsg, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, &reqs[0]);
    MPI_Isend(&rank, 1, MPI_INT, next, 0, MPI_COMM_WORLD, &reqs[1]);

    MPI_Waitall(2, reqs, stats); // Wait for both send and receive to complete

    MPI_Barrier(MPI_COMM_WORLD); // Synchronize all processes
    et = clock();
    if (rank == 0)
        printf("Time taken by NonBlocking send/receive: %lf seconds\n", (double)(et - st) / CLOCKS_PER_SEC);

    MPI_Finalize(); // Finalize MPI
    return 0;
}

//mpicc -o mpi_timing mpi_timing.c
//mpirun -np 4 ./mpi_timing
