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
void multiply_matrices_ijk(double **a, double **b, double **result, int n, int num_threads) {
    // Set the number of threads for this calculation
    omp_set_num_threads(num_threads);

    // Parallelize only the outermost loop 
    // #pragma omp parallel for
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double sum = 0.0;
            for (int k = 0; k < n; k++) {
                sum += a[i][k] * b[k][j];
            }
            result[i][j] = sum;
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
    // #pragma omp parallel for
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
    int threads[] = {1, 2, 4, 8, 16, 32}; // Number of threads
    size_t num_tests = sizeof(threads) / sizeof(threads[0]);

    size_t n = 2000; // Matrix size (2 * 10^3)

    srand(time(0)); // Seed the random number generator once

    // Allocate memory for matrices A, B, and result
    double **a = allocate_matrix(n);
    double **b = allocate_matrix(n);
    double **result = allocate_matrix(n);

    // Initialize the result matrix to zero
    initialize_matrix(result, n);

    double start, end;

    // Generate matrix A
    start = omp_get_wtime();
    generate_matrix(a, n);
    end = omp_get_wtime();
    printf("Time to generate matrix A: %lf seconds\n", end - start);

    // Generate matrix B
    start = omp_get_wtime();
    generate_matrix(b, n);
    end = omp_get_wtime();
    printf("Time to generate matrix B: %lf seconds\n", end - start);

    // Open a file to write the results
    FILE* file = fopen("matrix_multiplication_times.csv", "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file for writing\n");
        free_matrix(a, n);
        free_matrix(b, n);
        free_matrix(result, n);
        return 1;
    }

    // Write header to CSV file
    fprintf(file, "Threads,Execution Time (seconds)\n");

    // Perform matrix multiplication with different numbers of threads
    for (size_t i = 0; i < num_tests; i++) {
        int num_threads = threads[i];
        printf("Multiplying matrices with %d threads\n", num_threads);

        // Measure execution time
        start = omp_get_wtime();
        multiply_matrices_ijk(a, b, result, n, num_threads);
        end = omp_get_wtime();
        double execution_time = end - start;

        // Print and save the result
        printf("Time to multiply matrices (i-j-k) with %d threads: %lf seconds\n", num_threads, execution_time);
        fprintf(file, "%d,%f\n", num_threads, execution_time);
    }

    // Close the file and free memory
    fclose(file);
    free_matrix(a, n);
    free_matrix(b, n);
    free_matrix(result, n);

    return 0;
}