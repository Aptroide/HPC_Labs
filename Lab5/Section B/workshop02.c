#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h> // Include OpenMP header

// Function to generate a matrix with random double values
void generate_matrix(double **matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = (double)rand() / RAND_MAX;
        }
    }
}

// Function to multiply matrices with loop order i-j-k
// void multiply_matrices_ijk(double **a, double **b, double **result, int n) {
//     // Parallelize only the outermost loop
//     // #pragma omp parallel for
//     #pragma omp parallel for collapse(2)
//     for (int i = 0; i < n; i++) {
//         for (int j = 0; j < n; j++) {
//             double sum = 0.0;
//             for (int k = 0; k < n; k++) {
//                 sum += a[i][k] * b[k][j];
//             }
//             result[i][j] = sum;
//         }
//     }
// }

// Function to multiply matrices with loop order i-k-j
void multiply_matrices_ikj(double **a, double **b, double **result, int n) {
    // Parallelize only the outermost loop
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            double temp = a[i][k];
            for (int j = 0; j < n; j++) {
                result[i][j] += temp * b[k][j];
            }
        }
    }
}

// Function to allocate memory for a matrix using malloc
double **allocate_matrix(int n) {
    double **matrix = (double **)malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++) {
        matrix[i] = (double *)malloc(n * sizeof(double));
    }
    return matrix;
}

// Function to initialize a matrix to zero
void initialize_matrix(double **matrix, int n) {
    // Parallelize only the outermost loop
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = 0.0;
        }
    }
}

// Function to free the memory allocated to a matrix
void free_matrix(double **matrix, int n) {
    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

int main(int argc, char *argv[]) {


    // Set the number of OpenMP threads
    omp_set_num_threads(omp_get_max_threads());
    
    size_t n[] = {2000, 5000}; // Matrix sizes

    srand(time(0)); // Seed the random number generator once

    for (size_t i = 0; i < sizeof(n) / sizeof(n[0]); i++) {
        size_t matrix_size = n[i];
        size_t memory_per_matrix = matrix_size * matrix_size * sizeof(double);
        size_t total_memory = 3 * memory_per_matrix; // For matrices A, B, and result

        double memory_per_matrix_mb = (double)memory_per_matrix / (1024 * 1024);
        double total_memory_mb = (double)total_memory / (1024 * 1024);

        printf("Matrix size: %zu x %zu\n", matrix_size, matrix_size);
        printf("Memory used by one matrix: %.2f MB\n", memory_per_matrix_mb);
        printf("Total memory used by three matrices: %.2f MB\n", total_memory_mb);

        // Allocate memory for matrices A, B, and result
        double **a = allocate_matrix(matrix_size);
        double **b = allocate_matrix(matrix_size);
        double **result = allocate_matrix(matrix_size);

        // Initialize the result matrix to zero
        initialize_matrix(result, matrix_size);

        double start, end;

        // Generate matrix A
        start = omp_get_wtime();
        generate_matrix(a, matrix_size);
        end = omp_get_wtime();
        printf("Time to generate matrix A: %lf seconds\n", end - start);

        // Generate matrix B
        start = omp_get_wtime();
        generate_matrix(b, matrix_size);
        end = omp_get_wtime();
        printf("Time to generate matrix B: %lf seconds\n", end - start);

        // Multiply matrices using loop order i-j-k
        start = omp_get_wtime();
        multiply_matrices_ikj(a, b, result, matrix_size);
        end = omp_get_wtime();
        printf("Time to multiply matrices (i-j-k): %lf seconds\n", end - start);

        // Free allocated memory
        free_matrix(a, matrix_size);
        free_matrix(b, matrix_size);
        free_matrix(result, matrix_size);

        printf("------------------------------------------------------\n");
    }

    return 0;
}
