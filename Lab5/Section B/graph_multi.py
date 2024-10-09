import pandas as pd
import matplotlib.pyplot as plt

# Load the CSV file with the results
data = pd.read_csv("matrix_multiplication_times.csv")

# Plot the execution times for different numbers of threads
plt.figure(figsize=(10, 6))
plt.plot(data['Threads'], data['Execution Time (seconds)'], marker='o', linestyle='-', color='b')
plt.xlabel('Number of Threads')
plt.ylabel('Execution Time (seconds)')
plt.title('Matrix Multiplication Execution Time vs Number of Threads')
plt.grid(True)
plt.show()
