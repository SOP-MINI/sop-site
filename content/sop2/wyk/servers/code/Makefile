CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = -lrt

TARGETS = select_chat epoll_chat

.PHONY: all clean

all: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGETS) *.o