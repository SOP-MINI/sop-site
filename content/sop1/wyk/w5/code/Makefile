CC = gcc
CFLAGS = -Wall -g

TARGETS = kill basic_handler segv_handler handler long_handler basic_blocking blocking eintr partial_read sigatomic sigchld sigsuspend sigwait lab_example

.PHONY: all clean

all: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(TARGETS) *.o
