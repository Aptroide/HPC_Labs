#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <sys/time.h>
#include <openacc.h>

#define WIDTH 1920
#define HEIGHT 1080
#define MAX_ITER 1000

// Function to get the current time in milliseconds
double get_time_in_ms() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000.0) + (tv.tv_usec / 1000.0);
}

void mandelbrot(float *image) {
    // Parallelize outer loop with OpenACC
    #pragma acc parallel loop vector_length(256) collapse(2) copyout(image[0:WIDTH*HEIGHT])
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            float complex c = (x - WIDTH / 2.0) * 4.0 / WIDTH + 
                              ((y - HEIGHT / 2.0) * 4.0 / HEIGHT) * I;
            float complex z = 0;
            int iter = 0;
            while (cabs(z) < 2.0 && iter < MAX_ITER) {
                z = z * z + c;
                iter++;
            }
            image[y * WIDTH + x] = (float) iter / MAX_ITER;
        }
    }
}

int main() {
    float *image = (float *)malloc(WIDTH * HEIGHT * sizeof(float));
    
    // Measure time for GPU implementation
    double start_time_gpu = get_time_in_ms();
    
    // Start parallel region using OpenACC for GPU implementation
    #pragma acc data copyout(image[0:WIDTH*HEIGHT])
    {
        mandelbrot(image);
    }
    
    double end_time_gpu = get_time_in_ms();
    double gpu_time = end_time_gpu - start_time_gpu;
    printf("GPU time: %.3f ms\n", gpu_time);
    
    // Measure time for CPU implementation (serial)
    double start_time_cpu = get_time_in_ms();
    
    // Call the mandelbrot function without OpenACC for CPU
    mandelbrot(image);  // This will run on CPU if OpenACC is disabled
    
    double end_time_cpu = get_time_in_ms();
    double cpu_time = end_time_cpu - start_time_cpu;
    printf("CPU time: %.3f ms\n", cpu_time);
    
    // Free memory
    free(image);
    
    return 0;
}
