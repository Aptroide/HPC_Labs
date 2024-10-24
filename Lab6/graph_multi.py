import pandas as pd
import matplotlib.pyplot as plt

# Load the CSV files with the results
data1 = pd.read_csv("matrix_multiplication_times.csv")
data2 = pd.read_csv("matrix_multiplication_times_other.csv")  # Replace with your other data file

# Plot the execution times for different numbers of threads
plt.figure(figsize=(10, 6))
plt.plot(data1['Threads'], data1['Execution Time (seconds)'], marker='o', linestyle='-', color='b', label='Dataset 1')
plt.plot(data2['Threads'], data2['Execution Time (seconds)'], marker='s', linestyle='--', color='r', label='Dataset 2')
plt.xlabel('Number of Threads')
plt.ylabel('Execution Time (seconds)')
plt.title('Matrix Multiplication Execution Time vs Number of Threads')
plt.legend()
plt.grid(True)
plt.show()
