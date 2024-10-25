#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc == 1) {
    fprintf(stderr, "Error: file argument neede!\n");
    exit(1);
  }
  FILE *f = fopen(argv[1], "r");
  if (!f) {
    fprintf(stderr, "Error: could not open file \"%s\"\n", argv[1]);
    exit(1);
  }

  const int BUFSIZE = 65536;
  char buf[BUFSIZE], ch;
  // initialize histogram array
  int hist[256], most = -1;
  for (int i = 0; i < 256; i++) hist[i] = 0;

  // compute histogram
  while (1) {
    size_t size = fread(buf, 1, BUFSIZE, f);
    if (size <= 0) break;
    #pragma acc parallel loop copyin(buf[:size])
      for (int i = 0; i < size; i++) {
	int v = buf[i];
	#pragma acc atomic
	hist[v]++;
    }
  }
  // print the first highest peak
  for (int i = 0; i < 256; i++)
    if (hist[i] > most) {
      most = hist[i]; ch = i;
  }
  printf("Highest count of %d for character code %d\n", most, ch);

  return 0;
}