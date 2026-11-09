#include <stdio.h>
#include <stdlib.h>

int k = 1;
int tab[2] = {2, 3};

void stack(int n) {
  int a = n;
  printf("[n = %d] &a = %p\n", n, &a);
  if (n > 0)
    stack(n - 1);
}

void heap(int n) {
  for (int i = 0; i < n; ++i) {
    int *p = (int *)malloc(sizeof(int));
    printf("[i = %d] p = %p\n", i, p);
  }
}

int main(int argc, char **argv) {
  printf("argc = %d\n", argc);
  for (int i = 0; i < argc; ++i) {
    printf("argv[%d] = %s\n", i, argv[i]);
  }
  printf("Stack growth:\n");
  stack(5);
  printf("Heap growth:\n");
  heap(5);
  printf("Globals:\n");
  printf("&k = %p\n", &k);
  printf("&tab[0] = %p\n", &tab[0]);
  printf("&tab[1] = %p\n", &tab[1]);
  printf("Code:\n");
  printf("&main = %p\n", main);
  printf("&stack = %p\n", stack);
  printf("&heap = %p\n", heap);
  return 123;
}
