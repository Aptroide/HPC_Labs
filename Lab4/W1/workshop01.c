#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gperftools/profiler.h>
#include <gperftools/heap-profiler.h>

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
    // Start profiling and heap profiling
    ProfilerStart("profile.prof");
    HeapProfilerStart("heap_profile");

    size_t size = 2000000;

    double generation_time;
    double execution_time;

    // Measure array generation time
    clock_t start_generation_time = clock();
    
    double* array1 = generate_random_array(size);
    double* array2 = generate_random_array(size);
    
    clock_t end_generation_time = clock();
    generation_time = (double)(end_generation_time - start_generation_time) / CLOCKS_PER_SEC;
    printf("Generated arrays of size %zu in %f seconds\n", size, generation_time);

    // Measure dot product computation time
    printf("Computing dot product of arrays of size %zu\n", size);
    clock_t start_time = clock();
    
    double dot_product_result = dot_product(array1, array2, size);
    
    clock_t end_time = clock();
    execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Dot product of arrays of size %zu is %f in %f seconds\n", size, dot_product_result, execution_time);

    // Force heap dump before freeing memory
    HeapProfilerDump("dump_after_free");

    // Free memory
    free(array1);
    free(array2);

    // Stop the profiler and the heap profiling
    ProfilerStop();
    HeapProfilerStop();

    return 0;
}
