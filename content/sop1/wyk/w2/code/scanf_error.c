#include <stdio.h>
#include <unistd.h>

int main(int argc, const char **argv) {
  char buf[10];
  int age;
  int ret;

  printf("Your age? > ");
  ret = scanf("%d", &age);
  printf("scanf() returned %d, age = %d\n", ret, age);
  printf("feof() = %d, ferror() = %d\n", feof(stdin), ferror(stdin));

  printf("Your name? > ");
  ret = scanf("%9s", buf);
  printf("scanf() returned %d, name = %s\n", ret, buf);
  printf("feof() = %d, ferror() = %d\n", feof(stdin), ferror(stdin));
  return 0;
}
