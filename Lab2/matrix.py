import numpy as np
import time

def calculate_memory(n):
    element_size = np.dtype(np.float64).itemsize  # Size of a single float64 in NumPy
    memory_per_matrix = n * n * element_size
    total_memory = 3 * memory_per_matrix  # For A, B, and result matrices
    memory_per_matrix_mb = memory_per_matrix / (1024 * 1024)  # Convert to megabytes
    total_memory_mb = total_memory / (1024 * 1024)  # Convert to megabytes
    return memory_per_matrix_mb, total_memory_mb

if __name__ == "__main__":
    sizes = [1000, 2000, 5000, 10000]

    for n in sizes:
        print(f"Matrix size: {n} x {n}")

        memory_per_matrix, total_memory = calculate_memory(n)
        print(f"Memory used by one matrix: {memory_per_matrix} bytes")
        print(f"Total memory used by three matrices: {total_memory} bytes")

        start = time.time()
        a = np.random.rand(n, n)
        end = time.time()
        print(f"Time to generate matrix A: {end - start} seconds")

        start = time.time()
        b = np.random.rand(n, n)
        end = time.time()
        print(f"Time to generate matrix B: {end - start} seconds")

        start = time.time()
        result = np.dot(a, b)
        end = time.time()
        print(f"Time to multiply matrices: {end - start} seconds")
        
        print("\n")
