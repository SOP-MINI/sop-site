CC = gcc
CFLAGS = -Wall -Wextra -g -pthread
LDFLAGS = -lrt

TARGETS = dining_philosophers

.PHONY: all clean

all: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGETS) *.o