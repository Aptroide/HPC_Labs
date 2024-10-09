import numpy as np
import time

import matplotlib.pyplot as plt

def generate_random_array(length):
    """Generate a random array of given length with values in [0, 1]."""
    return np.random.random(length)

def calculate_dot_product(array1, array2):
    """Calculate the dot product of two arrays."""
    if len(array1) != len(array2):
        raise ValueError("Arrays must have the same length")
    return np.dot(array1, array2)

def main():
    # Array of different values of n
    n_values = [5*10**7, 10**8, 2*10**8]
    
    generation_times = []
    dot_product_times = []

    for n in n_values:
        print(f"\nFor n = {n}:")
        
        # Measure the time to generate arrays
        start_time = time.time()
        array1 = generate_random_array(n)
        array2 = generate_random_array(n)
        array_generation_time = time.time() - start_time
        generation_times.append(array_generation_time)

        # Measure the time to calculate the dot product
        start_time = time.time()
        dot_product = calculate_dot_product(array1, array2)
        dot_product_time = time.time() - start_time
        dot_product_times.append(dot_product_time)

        # Print results
        print(f"  Array generation time: {array_generation_time:.6f} seconds")
        print(f"  Dot product calculation time: {dot_product_time:.6f} seconds")
        print(f"  Dot product: {dot_product}")

    # # Create plots
    # plt.figure(figsize=(12, 6))
    
    # plt.subplot(1, 2, 1)
    # plt.plot(n_values, generation_times, 'b-o')
    # plt.title('Array Generation Time')
    # plt.xlabel('n')
    # plt.ylabel('Time (seconds)')
    # plt.xscale('log')
    
    # plt.subplot(1, 2, 2)
    # plt.plot(n_values, dot_product_times, 'r-o')
    # plt.title('Dot Product Calculation Time')
    # plt.xlabel('n')
    # plt.ylabel('Time (seconds)')
    # plt.xscale('log')
    
    # plt.tight_layout()
    # plt.show()

if __name__ == "__main__":
    main()


