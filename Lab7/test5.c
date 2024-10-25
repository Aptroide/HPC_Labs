#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  unsigned N = 1024;
  if (argc > 1) N = strtoul(argv[1], 0, 10);

  // create triangular matrix
  double **restrict m = malloc(N*sizeof(double *));
  for (int i = 0; i < N; i++)
  {
    m[i] = malloc(N*sizeof(double));
    for (int j = 0; j < N; j++)
      m[i][j] = (i > j)? 0: 1.0;
  }

  // create vector filled with ones
  double *restrict v = malloc(N*sizeof(double));
  for (int i = 0; i < N; i++) v[i] = 1.0;

  // create result vector
  double *restrict b = malloc(N*sizeof(double));

  // multiply in parallel
  #pragma acc kernels copyin(m[:N][:N], v[:N]) copyout(b[:N])
  for (int i = 0; i < N; i++)
  {
    b[i] = 0;
    for (int j = 0; j < N; j++)
      b[i] += m[i][j]*v[j];
  }

  // verify result
  double r = 0;
  for (int i = 0; i < N; i++) r += b[i];
  printf("Result: %f (expected %f)\n", r, (N+1)*N/2.0);

  return 0;
}