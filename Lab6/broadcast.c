#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int data;
    if (rank == 0) {
        data = 100; // Root process initializes the data
    }
    MPI_Bcast(&data, 1, MPI_INT, 0, MPI_COMM_WORLD); // Broadcast the data to all processes
    printf("Process %d received data %d\n", rank, data);
    MPI_Finalize();
    return 0;
}