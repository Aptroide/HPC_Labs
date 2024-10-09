import matplotlib.pyplot as plt

# Data extracted from the output text
threads = [1, 2, 4, 8, 16, 32]
execution_times = [0.315844, 0.165293, 0.126838, 0.133505, 0.118238, 0.117434]

# Plotting the data
plt.figure(figsize=(10, 6))
plt.plot(threads, execution_times, marker='o', linestyle='-', color='b')
plt.xlabel('Number of Threads')
plt.ylabel('Execution Time (seconds)')
plt.title('Dot Product Execution Time vs Number of Threads')
plt.grid(True)
plt.show()
