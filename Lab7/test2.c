#include <stdio.h>

const int N = 1000;

int main() {
  int vec[N];
  int cpu_sum = 0, gpu_sum = 0;

  // initialization
  for (int i = 0; i < N; i++) {
    vec[i] = i+1;
  }
  #pragma acc parallel async
  for (int i = 100; i < N; i++) {
    gpu_sum += vec[i];
  }
  // the following code executes without waiting for GPU result
  for (int i = 0; i < 100; i++) {
    cpu_sum += vec[i];
  }
  // synchronize and verify results
  
  #pragma acc wait
  printf("%d %d\n", cpu_sum, (100+1)*100/2);
  printf("%d %d\n", gpu_sum, (N+1)*N/2 - (100+1)*100/2);
  printf("Result: %d (expected: %d)\n", gpu_sum+cpu_sum, (N+1)*N/2);

  return 0;
}