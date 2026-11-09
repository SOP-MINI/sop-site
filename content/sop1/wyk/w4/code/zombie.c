#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/wait.h>

int main(int argc, const char **argv) {
  printf("Hello, world!\n");

  if (argc < 2) {
    printf("Gimme process num :(\n");
    return 1;
  }

  int n = atoi(argv[1]);

  for (int i = 0; i < n; ++i) {
    switch (fork()) {
    case -1:
      printf("Fork failed :(\n");
      return 2;
    case 0:
      // Child sleeps 1s and exits with its index as status
      printf("Child %d works\n", i);
      sleep(5);
      printf("Child %d exits\n", i);
      exit(i + 1);
    default:
      // Parent continues
      break;
    }
  }

  while (1) {
    char cmd[16];
    printf("What do you want? > ");
    fflush(stdout);                // TODO: Why is it needed?
    if (scanf("%15s", cmd) == 1) { // TODO: Why 15? What scanf() returns?
      if (strcmp(cmd, "exit") == 0)
        break;
      printf("Ok, doing \"%s\" ...", cmd);
      fflush(stdout);
      sleep(1);
      printf(" done!\n");
    }
  }

  printf("Parent exiting\n");

  for (int i = 0; i < n; ++i) {
    int status;
    wait(&status);
  }

  return 0;
}
