#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, const char **argv) {
  ssize_t ret;
  char buf[10]; // 'a', 's', 'd', 'f', '\n', ?, ?, ?, ?, ?

  // Low-level version of printf("Name? > ");
  write(STDOUT_FILENO, "Name? >", 7);
  // LL version of scanf("%9s", buf);
  ret = read(STDIN_FILENO, buf, sizeof(buf));

  buf[ret] = '\0';

  char text[100];
  int s = snprintf(text, sizeof(text), "Read returned %ld\n", ret);
  write(STDOUT_FILENO, text, s);

  // LL version of printf("Hello %s", buf);
  write(STDOUT_FILENO, "Hello ", 6);
  write(STDOUT_FILENO, buf, strlen(buf));

  return 0;
}
