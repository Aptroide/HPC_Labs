#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Function to generate an array with random values
double* generate_random_array(size_t size) {
    double* array = (double*)malloc(size * sizeof(double));
    for (size_t i = 0; i < size; i++) {
        array[i] = (double)rand() / RAND_MAX;
    }
    return array;
}

// Function to calculate the dot product of two arrays
double dot_product(double* array1, double* array2, size_t size, int num_threads) {
    double result = 0.0;

    // Set the number of threads for this calculation
    omp_set_num_threads(num_threads);

    // Parallel reduction for dot product computation
    #pragma omp parallel for reduction(+:result)
    for (size_t i = 0; i < size; i++) {
        result += array1[i] * array2[i];
    }

    return result;
}

int main() {
    size_t n = 1e8; // n = 10^8
    int threads[] = {1, 2, 4, 8, 16, 32};
    size_t num_tests = sizeof(threads) / sizeof(threads[0]);

    // Generate the arrays
    double* array1 = generate_random_array(n);
    double* array2 = generate_random_array(n);

    // Open a file to write the results
    FILE* file = fopen("dot_product_times.csv", "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file for writing\n");
        free(array1);
        free(array2);
        return 1;
    }

    // Write header to CSV file
    fprintf(file, "Threads,Execution Time (seconds)\n");

    // Perform dot product with different numbers of threads
    for (size_t i = 0; i < num_tests; i++) {
        int num_threads = threads[i];
        printf("Computing dot product with %d threads\n", num_threads);

        // Measure execution time
        double start_time = omp_get_wtime();
        double dot_product_result = dot_product(array1, array2, n, num_threads);
        double end_time = omp_get_wtime();
        double execution_time = end_time - start_time;

        // Print and save the result
        printf("Dot product with %d threads is %f in %f seconds\n", num_threads, dot_product_result, execution_time);
        fprintf(file, "%d,%f\n", num_threads, execution_time);
    }

    // Close the file and free memory
    fclose(file);
    free(array1);
    free(array2);

    return 0;
}