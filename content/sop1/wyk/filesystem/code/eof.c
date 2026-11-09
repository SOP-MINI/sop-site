#include <stdio.h>

int main(int argc, char **argv) {
  FILE *f = stdin;
  if (argc > 1) {
    f = fopen(argv[1], "r");
    if (!f) {
      perror("fopen()");
      return 1;
    }
  }

  char buf[10] = {0};

  while (!feof(f)) {
    int ret = fscanf(f, "%9s", buf);
    printf("fscanf() returned %d, buf = \"%s\", feof(f) = %d\n", ret, buf,
           feof(f));
  }

  return 0;
}
