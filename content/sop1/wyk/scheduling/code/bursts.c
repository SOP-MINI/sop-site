#define _GNU_SOURCE

#include <assert.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ARR_SIZE (4ul * 1024 * 1024 * 1024)

// #define CPU_AFFINITY 2

int main(int argc, char **argv) {

#ifdef CPU_AFFINITY
  cpu_set_t set;
  CPU_ZERO(&set);
  CPU_SET(CPU_AFFINITY, &set);
  if (sched_setaffinity(getpid(), sizeof(set), &set) == -1) {
    perror("sched_setaffinity");
    return 1;
  }
#endif

  char *arr = (char *)malloc(ARR_SIZE);
  assert(arr != NULL);

  char cmd[64];
  printf("cmd> ");
  fflush(stdout);
  while (scanf("%63s", cmd) == 1) {

    if (strcmp(cmd, "exit") == 0) {
      break;
    }

    printf("Executing '%s' ...\n", cmd);
    for (int i = 0; i < strlen(cmd); ++i) {
      memset(arr, cmd[i], ARR_SIZE);
    }
    printf("Done '%s' ...\ncmd>", cmd);
    fflush(stdout);
  }

  free(arr);
  return 0;
}
