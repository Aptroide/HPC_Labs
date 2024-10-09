#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

double* generate_random_array(size_t size) {
    double* array = (double*)malloc(size * sizeof(double));
    for (size_t i = 0; i < size; i++) {
        array[i] = (double)rand() / RAND_MAX;
    }
    return array;
}

double dot_product(double* array1, double* array2, size_t size) {
    double result = 0.0;

    // Parallel reduction for dot product computation
    #pragma omp parallel for reduction(+:result)
    for (size_t i = 0; i < size; i++) {
        result += array1[i] * array2[i];
    }

    return result;
}

int main() {
    size_t n[] = {5e7, 1e8, 2e8};
    size_t num_tests = sizeof(n) / sizeof(n[0]);
    
    double generation_times[num_tests];
    double execution_times[num_tests];

    // Set the number of threads to the maximum available
    omp_set_num_threads(omp_get_max_threads());

    for (size_t i = 0; i < num_tests; i++) {
        size_t size = n[i];

        // Measure array generation time
        double start_generation_time = omp_get_wtime();
        
        double* array1 = generate_random_array(size);
        double* array2 = generate_random_array(size);
        
        double end_generation_time = omp_get_wtime();
        double generation_time = end_generation_time - start_generation_time;
        printf("Generated arrays of size %zu in %f seconds\n", size, generation_time);
        generation_times[i] = generation_time;

        // Measure dot product computation time
        printf("Computing dot product of arrays of size %zu\n", size);
        double start_time = omp_get_wtime();
        
        double dot_product_result = dot_product(array1, array2, size);
        
        double end_time = omp_get_wtime();
        double execution_time = end_time - start_time;
        execution_times[i] = execution_time;
        printf("Dot product of arrays of size %zu is %f in %f seconds\n", size, dot_product_result, execution_time);

        free(array1);
        free(array2);
    }

    return 0;
}


// Original Code

/*  
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double* generate_random_array(size_t size) {
    double* array = (double*)malloc(size * sizeof(double));
    for (size_t i = 0; i < size; i++) {
        array[i] = (double)rand() / RAND_MAX;
    }
    return array;
}

double dot_product(double* array1, double* array2, size_t size) {
    double result = 0.0;
    for (size_t i = 0; i < size; i++) {
        result += array1[i] * array2[i];
    }
    return result;
}

int main() {
    size_t n[] = {5e7, 1e8, 2e8};

    size_t num_tests = sizeof(n) / sizeof(n[0]);
    
    double generation_times[num_tests];
    double execution_times[num_tests];

    for (size_t i = 0; i < num_tests; i++) {
        size_t size = n[i];

        // Measure array generation time
        clock_t start_generation_time = clock();
        
        double* array1 = generate_random_array(size);
        double* array2 = generate_random_array(size);
        
        clock_t end_generation_time = clock();
        double generation_time = (double)(end_generation_time - start_generation_time) / CLOCKS_PER_SEC;
        printf("Generated arrays of size %zu in %f seconds\n", size, generation_time);
        generation_times[i] = generation_time;

        // Measure dot product computation time
        printf("Computing dot product of arrays of size %zu\n", size);
        clock_t start_time = clock();
        
        double dot_product_result = dot_product(array1, array2, size);
        
        clock_t end_time = clock();
        double execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
        execution_times[i] = execution_time;
        printf("Dot product of arrays of size %zu is %f in %f seconds\n", size, dot_product_result, execution_time);

        free(array1);
        free(array2);
    }

    return 0;
}
*/