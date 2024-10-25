#include <stdio.h>
#include <stdlib.h>

const int N = 10000;

int main() {
  double x[N], y[N];
  double a = 2.0, r = 0.0;

  #pragma acc kernels
  {
    // initialize the vectors
    #pragma acc loop gang worker
    for (int i = 0; i < N; i++) {
      x[i] = 1.0;
      y[i] = -1.0;
    }

    // perform computation
    #pragma acc loop independent reduction(+:r)
    for (int i = 0; i < N; i++) {
      y[i] = a*x[i]+y[i];
      r += y[i];
    }
  }

  // print result
  printf("Result: %f (expected %f)\n", r, (float)N);

  return 0;
}