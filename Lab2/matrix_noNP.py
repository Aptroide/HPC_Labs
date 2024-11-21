import random
import time
import sys

def generate_matrix(n):
    return [[random.random() for _ in range(n)] for _ in range(n)]

def multiply_matrices(a, b, n):
    result = [[0.0] * n for _ in range(n)]
    for i in range(n):
        for k in range(n):
            temp = a[i][k]
            for j in range(n):
                result[i][j] += temp * b[k][j]
    return result

def calculate_memory(n):
    element_size = sys.getsizeof(0.0)
    memory_per_matrix = n * n * element_size
    total_memory = 3 * memory_per_matrix
    memory_per_matrix_mb = memory_per_matrix / (1024 * 1024)
    total_memory_mb = total_memory / (1024 * 1024)
    return memory_per_matrix_mb, total_memory_mb

if __name__ == "__main__":
    sizes = [1000, 2000, 5000, 10000]

    for n in sizes:
        print(f"Matrix size: {n} x {n}")

        memory_per_matrix, total_memory = calculate_memory(n)
        print(f"Memory used by one matrix: {memory_per_matrix} MB")
        print(f"Total memory used by three matrices: {total_memory} MB")

        start = time.time()
        a = generate_matrix(n)
        end = time.time()
        print(f"Time to generate matrix A: {end - start} seconds")

        start = time.time()
        b = generate_matrix(n)
        end = time.time()
        print(f"Time to generate matrix B: {end - start} seconds")

        start = time.time()
        result = multiply_matrices(a, b, n)
        end = time.time()
        print(f"Time to multiply matrices: {end - start} seconds")
        
        print("\n")
