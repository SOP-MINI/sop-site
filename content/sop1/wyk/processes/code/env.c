#include <stdio.h>
#include <stdlib.h>

extern char **environ;

int main(int argc, char **argv) {
  for (int i = 0; i < argc; ++i) {
    printf("argv[%d] = %s\n", i, argv[i]);
  }

  char **p = environ;
  while (*p) {
    printf("%s\n", *p); // TODO: Uncomment
    p++;
  }

  char *ops = getenv("OPS");
  if (ops) {
    printf("OPS is set to = %s\n", ops);
  } else {
    printf("No OPS :(\n");
  }

  return 0;
}
