#include <stdio.h>

int main(int argc, const char **argv) {
  int i = 0;
  while (1) {
    FILE *f = fopen("/tmp/stream_limit.txt~", "w");
    if (!f) {
      printf("FOPEN_MAX = %d, i = %d\n", FOPEN_MAX, i);
      perror("fopen()");
      break;
    }
    i++;
  }
  getchar();
  return i;
}
