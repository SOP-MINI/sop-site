#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, const char **argv) {
  printf("PID = %d\n", getpid());
  int i = 0;
  while (1) {
    int fd = open("/tmp/descriptor_limit.txt~", O_CREAT | O_WRONLY, 0777);
    if (fd < 0) {
      printf("i = %d\n", i);
      perror("open()");
      break;
    }
    i++;
  }
  getchar();
  return 0;
}
