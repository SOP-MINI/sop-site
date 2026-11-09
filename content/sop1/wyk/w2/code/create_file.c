#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#ifndef MODE
#define MODE 0600
#endif

int main(int argc, const char **argv) {
  if (argc != 2) {
    fprintf(stderr, "No file name provided!\n");
    return 1;
  }

#ifdef UMASK
  mode_t old = umask(UMASK);
#endif

  // TODO: Try to remove 3'rd arg
  int fd = open(argv[1], O_CREAT | O_WRONLY, MODE);

#ifdef UMASK
  umask(old);
#endif

  if (fd >= 0) {
    close(fd);
    return 0;
  } else {
    perror("open(): ");
    return 1;
  }
}
