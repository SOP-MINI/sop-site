CC = gcc
CFLAGS = -Wall -Wextra

TARGETS = stackprint

.PHONY: all clean

all: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(TARGETS) *.o