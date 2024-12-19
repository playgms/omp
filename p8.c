#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define SIZE 16 // Total number of processes
#define UP 0    // Direction indices
#define DOWN 1
#define LEFT 2
#define RIGHT 3

int main(int argc, char* argv[]) {
    int numtasks, rank, source, dest, outbuf, i, tag = 1;
    int inbuf[4] = { MPI_PROC_NULL, MPI_PROC_NULL, MPI_PROC_NULL, MPI_PROC_NULL };
    int nbrs[4], dims[2] = { 4, 4 }, periods[2] = { 0, 0 }, reorder = 0, coords[2];
    MPI_Request reqs[8];
    MPI_Status stats[8];
    MPI_Comm cartcomm;

    MPI_Init(&argc, &argv);                   // Initialize MPI
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks); // Get the number of processes

    if (numtasks == SIZE) {
        // Create a 2D Cartesian topology
        MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, reorder, &cartcomm);

        // Get the rank and coordinates in the Cartesian topology
        MPI_Comm_rank(cartcomm, &rank);
        MPI_Cart_coords(cartcomm, rank, 2, coords);

        // Find the neighbors in all four directions
        MPI_Cart_shift(cartcomm, 0, 1, &nbrs[UP], &nbrs[DOWN]);
        MPI_Cart_shift(cartcomm, 1, 1, &nbrs[LEFT], &nbrs[RIGHT]);

        // Print the rank, coordinates, and neighbors
        printf("rank= %d coords= %d %d neighbors(u,d,l,r)= %d %d %d %d\n",
               rank, coords[0], coords[1], nbrs[UP], nbrs[DOWN], nbrs[LEFT], nbrs[RIGHT]);

        // Send and receive data to/from neighbors
        outbuf = rank;
        for (i = 0; i < 4; i++) {
            dest = nbrs[i];
            source = nbrs[i];
            MPI_Isend(&outbuf, 1, MPI_INT, dest, tag, cartcomm, &reqs[i]);
            MPI_Irecv(&inbuf[i], 1, MPI_INT, source, tag, cartcomm, &reqs[i + 4]);
        }

        // Wait for all non-blocking operations to complete
        MPI_Waitall(8, reqs, stats);

        // Print the data received from neighbors
        printf("rank= %d inbuf(u,d,l,r)= %d %d %d %d\n",
               rank, inbuf[UP], inbuf[DOWN], inbuf[LEFT], inbuf[RIGHT]);
    } else {
        // Ensure the program is run with the correct number of processes
        printf("Must specify %d tasks. Terminating.\n", SIZE);
    }

    MPI_Finalize(); // Finalize MPI
    return 0;
}


//mpicc -o cartesian_topology cartesian_topology.c
//mpirun -np <put_the_value_of_SIZE> ./cartesian_topology
