#include <stdio.h>
#include <unistd.h>
#include <time.h>

int main(int argc, const char **argv) {
  FILE *f = stdout;
  if (argc == 2) {
    f = fopen(argv[1], "r");
    if (!f) return 1;
  }
#ifdef BUFF_MODE
  setvbuf(f, NULL, BUFF_MODE, 0);
#endif
  for (int i = 0; i < 40; ++i) {
    for (int j = 0; j < 30; ++j) {
      fprintf(f, "nice%d, ", i);
      struct timespec ts = {0, 10 * 1000 * 1000};
      nanosleep(&ts, NULL);
    }
    fprintf(f, "\n");
  }
  return 0;
}
