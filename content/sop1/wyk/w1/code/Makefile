CC = gcc
CFLAGS = -Wall

TARGETS = hello hello32 hello64 fizzbuzz_buffered fizzbuzz_direct

.PHONY: all clean

all: $(TARGETS)

hello: hello.c
	$(CC) $(CFLAGS) $< -o $@

hello32: hello32.asm
	nasm -f elf32 $< -o hello32.o
	$(CC) -m32 -o $@ -nostartfiles -nostdlib -nodefaultlibs -nolibc -static -OS hello32.o

hello64: hello64.asm
	nasm -f elf64 $< -o hello64.o
	$(CC) -o $@ -nostartfiles -nostdlib -nodefaultlibs -nolibc -static -OS hello64.o

fizzbuzz_buffered: fizzbuzz_buffered.c
	$(CC) -O2 $< -o $@

fizzbuzz_direct: fizzbuzz_direct.c
	$(CC) -O2 $< -o $@

clean:
	rm -f $(TARGETS) *.o
