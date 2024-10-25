#include <stdio.h>
#include <stdlib.h>

const int N = 500;

int main() {
  // initialize triangular matrix
  double m[N][N];
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      m[i][j] = (i > j)? 0: 1.0;

  // initialize input vector to all ones
  double v[N];
  for (int i = 0; i < N; i++) v[i] = 1.0;

  // initialize result vector
  double b[N];
  for (int i = 0; i < N; i++) b[i] = 0;

  // multiply in parallel
  #pragma acc kernels
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      b[i] += m[i][j]*v[j];

  // verify result
  double r = 0;
  for (int i = 0; i < N; i++) r += b[i];
  printf("Result: %f (expected %f)\n", r, (N+1)*N/2.0);

  return 0;
}