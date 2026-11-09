CC = gcc
CFLAGS = -Wall -g -pthread

TARGETS = 01_semaphore_base 07_semaphore 11_sem_open 14_cv_base 15_cv 20_barrier

.PHONY: all clean

all: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

15_cv: 15_cv.c
	$(CC) $(CFLAGS) -fsanitize=thread $< -o $@

20_barrier: 20_barrier.c
	$(CC) $(CFLAGS) -fsanitize=thread $< -o $@

clean:
	rm -f $(TARGETS) *.o
