CC = gcc
CFLAGS = -Wall -Wextra

TARGETS = hello hello_shared hello_static libshared_utils.so

.PHONY: all clean

all: $(TARGETS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

libshared_utils.so: shared_utils.c
	$(CC) $^ -o $@ -shared -fPIC

hello: hello.o main.o
	$(CC) $^ -o $@

hello_shared: hello.o main_shared.o libshared_utils.so
	$(CC) hello.o main_shared.o -o $@ -L. -lshared_utils

hello_static: hello.o main.o
	$(CC) -static $^ -o $@

clean:
	rm -f $(TARGETS) *.o