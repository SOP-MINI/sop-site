#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/uio.h>

struct node {
  char name[16];
  struct node* next;
};

struct node* allocate_node(char* name, struct node* next) {
  struct node* ptr = malloc(sizeof(struct node));
  if (!ptr) {
    perror("malloc");
    exit(1);
  }
  memset(ptr, 0, sizeof(struct node));
  strncpy(ptr->name, name, sizeof(ptr->name));
  ptr->next = next;
  return ptr;
}

void write_list(int fd, struct node* head) {

  struct iovec iov[10];
  int i = 0;

  while (head) {
    iov[i].iov_base = head->name;
    iov[i].iov_len = strlen(head->name);
    head = head->next;
    i++;
  }

  ssize_t ret = writev(fd, iov, i);
  printf("writev() written %ld bytes\n", ret);
}

void free_list(struct node* head) {
  while (head) {
    struct node* next = head->next;
    free(head);
    head = next;
  }
}

int main(int argc, const char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Filename required!\n");
    return 1;
  }

  struct node* head = allocate_node("last", NULL);
  head = allocate_node("mid", head);
  head = allocate_node("first", head);

  int fd = open(argv[1], O_CREAT | O_TRUNC | O_WRONLY, 0644);
  if (fd < 0) {
    perror("open");
    free_list(head);
    return 1;
  }

  write_list(fd, head);

  free_list(head);
  return 0;
}
