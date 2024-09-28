#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gperftools/profiler.h>
#include <gperftools/heap-profiler.h>

// Function to generate a matrix with random values
void generate_matrix(double **matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = (double)rand() / RAND_MAX;
        }
    }
}

// Function to multiply matrices using loop order i-k-j
void multiply_matrices_ikj(double **a, double **b, double **result, int n) {
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            double aik = a[i][k];
            for (int j = 0; j < n; j++) {
                result[i][j] += aik * b[k][j];
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

int main() {
    // Start the CPU profiler
    ProfilerStart("profile.prof");
    
    // Start the heap profiler
    HeapProfilerStart("heap_profile");

    int n = 2000;

    srand(time(0));

    size_t memory_per_matrix = n * n * sizeof(double);
    size_t total_memory = 3 * memory_per_matrix; // For matrices A, B, and result

    double memory_per_matrix_mb = (double)memory_per_matrix / (1024 * 1024);
    double total_memory_mb = (double)total_memory / (1024 * 1024);

    printf("Matrix size: %d x %d\n", n, n);
    printf("Memory used by one matrix: %.2f MB\n", memory_per_matrix_mb);
    printf("Total memory used by three matrices: %.2f MB\n", total_memory_mb);

    // Allocate memory for matrices A, B, and result
    double **a = allocate_matrix(n);
    double **b = allocate_matrix(n);
    double **result = allocate_matrix(n);

    // Initialize the result matrix to zero
    initialize_matrix(result, n);

    clock_t start, end;

    // Generate matrix A
    start = clock();
    generate_matrix(a, n);
    end = clock();
    printf("Time to generate matrix A: %lf seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    // Generate matrix B
    start = clock();
    generate_matrix(b, n);
    end = clock();
    printf("Time to generate matrix B: %lf seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    // Multiply matrices using loop order i-k-j
    start = clock();
    multiply_matrices_ikj(a, b, result, n);
    end = clock();
    printf("Time to multiply matrices (i-k-j): %lf seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    // Force heap dump before freeing memory
    HeapProfilerDump("dump_after_multiplication");

    // Free allocated memory
    free_matrix(a, n);
    free_matrix(b, n);
    free_matrix(result, n);

    // Stop the heap profiler
    HeapProfilerStop();
    
    // Stop the CPU profiler
    ProfilerStop();

    return 0;
}
