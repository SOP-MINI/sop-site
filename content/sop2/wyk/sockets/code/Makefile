CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = -lrt

TARGETS =

.PHONY: all clean

all: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGETS) *.o