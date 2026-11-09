#include <stdio.h>
#include <unistd.h>

int main(int argc, const char **argv) {

  FILE *f = stdout;

  if (argc > 1) {
    f = fopen(argv[1], "w");
    if (f == NULL) {
      fprintf(stderr, "fopen(%s) failed :(\n", argv[1]);
      return 1;
    }
  }

  printf("ftell() returned %ld\n", ftell(f));
  fprintf(f, "Hello from %d\n", getpid());
  printf("ftell() returned %ld\n>", ftell(f));

  getchar();

  fseek(f, 0, SEEK_SET);

  printf("ftell() returned %ld\n", ftell(f));
  fprintf(f, "Bye from %d\n", getpid());
  printf("ftell() returned %ld\n>", ftell(f));

  return 0;
}
