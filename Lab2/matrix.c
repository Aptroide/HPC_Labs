#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void generate_matrix(double **matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = (double)rand() / RAND_MAX;
        }
    }
}


void multiply_matrices(double **a, double **b, double **result, int n) {
    register double sum;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            sum = 0.0;
            for (int k = 0; k < n; k++) {
                sum += a[i][k] * b[k][j];
            }
            result[i][j] = sum;
        }
    }
}

double **allocate_matrix(int n) {
    double **matrix = (double **)malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++) {
        matrix[i] = (double *)malloc(n * sizeof(double));
    }
    return matrix;
}

void free_matrix(double **matrix, int n) {
    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

int main() {
    int sizes[] = {1000, 2000, 5000, 10000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    srand(time(0));

    for (int s = 0; s < num_sizes; s++) {
        int n = sizes[s];
        size_t memory_per_matrix = n * n * sizeof(double);
        size_t total_memory = 3 * memory_per_matrix; // For A, B, and result matrices

        double memory_per_matrix_mb = (double)memory_per_matrix / (1024 * 1024);
        double total_memory_mb = (double)total_memory / (1024 * 1024);

        printf("Matrix size: %d x %d\n", n, n);
        printf("Memory used by one matrix: %.2f MB\n", memory_per_matrix_mb);
        printf("Total memory used by three matrices: %.2f MB\n", total_memory_mb);


        double **a = allocate_matrix(n);
        double **b = allocate_matrix(n);
        double **result = allocate_matrix(n);

        clock_t start, end;

        // Generate matrix a
        start = clock();
        generate_matrix(a, n);
        end = clock();
        printf("Time to generate matrix A: %lf seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

        // Generate matrix b
        start = clock();
        generate_matrix(b, n);
        end = clock();
        printf("Time to generate matrix B: %lf seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

        // Multiply matrices
        start = clock();
        multiply_matrices(a, b, result, n);
        end = clock();
        printf("Time to multiply matrices: %lf seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

        // Free allocated memory
        free_matrix(a, n);
        free_matrix(b, n);
        free_matrix(result, n);

        printf("\n");
    }

    return 0;
}
