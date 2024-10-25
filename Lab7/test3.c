#include <stdio.h>

int main() {
    int N = 1000;
    float a[N], b[N], c[N];

    // Initialize arrays
    for (int i = 0; i < N; i++) {
        a[i] = i * 1.0f;
        b[i] = i * 2.0f;
    }

    #pragma acc parallel loop
    for (int i = 0; i < N; i++) {
        c[i] = a[i] + b[i];
    }

    printf("c[0] = %f\n", c[1]);
    return 0;
}
