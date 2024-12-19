#include <stdio.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    int numtasks, rank;
    int dims[2], periods[2] = {0, 0}, coords[2];
    MPI_Comm cartcomm;

    MPI_Init(&argc, &argv);                   // Initialize MPI
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks); // Get the number of processes
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);     // Get the rank of this process

    // Define a 2D Cartesian grid
    dims[0] = 2;             // Rows
    dims[1] = numtasks / 2;  // Columns (adjust to the number of tasks)

    // Create the Cartesian topology
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &cartcomm);

    // Get the Cartesian coordinates of this process
    MPI_Cart_coords(cartcomm, rank, 2, coords);

    // Print the rank and coordinates
    printf("Rank %d: Coordinates = (%d, %d)\n", rank, coords[0], coords[1]);

    MPI_Finalize(); // Finalize MPI
    return 0;
}



//mpicc -o cartesian_topology cartesian_topology.c
//mpirun -np <put_the_value_of_SIZE> ./cartesian_topology
