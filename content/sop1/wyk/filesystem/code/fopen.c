#include <stdio.h>
#include <unistd.h>

int main(int argc, const char **argv) {
  printf("pid = %d\n", getpid());

  FILE *f = fopen("/tmp/fopen.txt", "w");
  if (!f) {
    perror("fopen()");
    return 1;
  }

  printf("fileno(f) = %d\n", fileno(f));
  printf("fileno(stdout) = %d\n", fileno(stdout));

  getchar();

  fprintf(f, "Hello from process %d\n", getpid());
  fprintf(stdout, "Hello from process %d\n", getpid());

  getchar();

  fclose(f);
  fclose(stdout); // Why not?

  getchar();

  printf("This should fail?\n");
  return 0;
}
