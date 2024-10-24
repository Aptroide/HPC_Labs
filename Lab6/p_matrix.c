#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to generate a matrix with random double values
void generate_matrix(double *matrix, int n) {
    for (int i = 0; i < n * n; i++) {
        matrix[i] = (double)rand() / RAND_MAX;
    }
}

// Function to multiply matrices: C = A * B
void multiply_matrices(const double *A, const double *B, double *C, int rows_per_process, int N) {
    for (int i = 0; i < rows_per_process; i++) {
        for (int j = 0; j < N; j++) {
            double sum = 0.0;
            for (int k = 0; k < N; k++) {
                sum += A[i * N + k] * B[k * N + j];
            }
            C[i * N + j] = sum;
        }
    }
}

// Function to allocate memory for a matrix using malloc
double *allocate_matrix(int n) {
    double *matrix = (double *)malloc(n * n * sizeof(double));
    if (matrix == NULL) {
        fprintf(stderr, "Error allocating memory for matrix.\n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
    return matrix;
}

// Function to initialize a matrix to zero
void initialize_matrix(double *matrix, int n) {
    for (int i = 0; i < n * n; i++) {
        matrix[i] = 0.0;
    }
}

int main(int argc, char *argv[]) {
    int rank, size;
    int N_values[] = {2000}; // Matrix sizes
    int num_sizes = sizeof(N_values) / sizeof(N_values[0]);

    // Initialize MPI environment
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Seed the random number generator differently on each process
    srand(time(NULL) + rank);

    for (int s = 0; s < num_sizes; s++) {
        int N = N_values[s];
        int rows_per_process = N / size;
        int remainder = N % size;

        // Handle the case where N is not perfectly divisible by size
        int *sendcounts = NULL;
        int *displs = NULL;
        if (rank == 0) {
            sendcounts = (int *)malloc(size * sizeof(int));
            displs = (int *)malloc(size * sizeof(int));
            for (int i = 0; i < size; i++) {
                sendcounts[i] = (i < remainder) ? (rows_per_process + 1) * N : rows_per_process * N;
                displs[i] = (i == 0) ? 0 : displs[i - 1] + sendcounts[i - 1];
            }
        }

        // Each process determines its number of rows
        int local_rows = (rank < remainder) ? (rows_per_process + 1) : rows_per_process;

        // Allocate memory for local_A and local_C
        double *local_A = (double *)malloc(local_rows * N * sizeof(double));
        double *local_C = (double *)malloc(local_rows * N * sizeof(double));
        if (local_A == NULL || local_C == NULL) {
            fprintf(stderr, "Error allocating memory for local matrices.\n");
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }

        // Root process allocates and initializes matrices A, B, and C
        double *matrix_A = NULL;
        double *matrix_B = (double *)malloc(N * N * sizeof(double));
        double *matrix_C = NULL;

        if (rank == 0) {
            matrix_A = allocate_matrix(N);
            matrix_B = allocate_matrix(N); // Allocate memory for matrix B
            matrix_C = allocate_matrix(N);

            // Generate matrices A and B
            generate_matrix(matrix_A, N);
            generate_matrix(matrix_B, N);
        } else {
            // Allocate memory for matrix B on non-root processes
            matrix_B = allocate_matrix(N);
        }

        // Broadcast matrix B to all processes
        MPI_Bcast(matrix_B, N * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        // Scatter the rows of matrix A to all processes
        MPI_Scatterv(matrix_A, sendcounts, displs, MPI_DOUBLE, local_A, local_rows * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        // Initialize local_C to zero
        initialize_matrix(local_C, local_rows);

        // Synchronize before starting the computation
        MPI_Barrier(MPI_COMM_WORLD);
        double start_time = MPI_Wtime();

        // Perform local matrix multiplication
        multiply_matrices(local_A, matrix_B, local_C, local_rows, N);

        // Synchronize after computation
        MPI_Barrier(MPI_COMM_WORLD);
        double end_time = MPI_Wtime();

        // Gather the results back to the root process
        MPI_Gatherv(local_C, local_rows * N, MPI_DOUBLE, matrix_C, sendcounts, displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        // Root process prints the timing and, optionally, the result matrix
        if (rank == 0) {
            size_t memory_per_matrix = N * N * sizeof(double);
            size_t total_memory = 3 * memory_per_matrix; // For matrices A, B, and C

            double memory_per_matrix_mb = (double)memory_per_matrix / (1024 * 1024);
            double total_memory_mb = (double)total_memory / (1024 * 1024);

            printf("Matrix size: %d x %d\n", N, N);
            printf("Memory used by one matrix: %.2f MB\n", memory_per_matrix_mb);
            printf("Total memory used by three matrices: %.2f MB\n", total_memory_mb);
            printf("Time to multiply matrices: %lf seconds\n", end_time - start_time);
            printf("------------------------------------------------------\n");

        }

        // Free allocated memory
        if (rank == 0) {
            free(matrix_A);
            free(matrix_B);
            free(matrix_C);
            free(sendcounts);
            free(displs);
        } else {
            free(matrix_B);
        }

        free(local_A);
        free(local_C);
    }

    // Finalize MPI environment
    MPI_Finalize();
    return 0;
}
