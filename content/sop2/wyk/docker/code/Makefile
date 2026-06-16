CC = gcc
CFLAGS = -Wall -Wextra

TARGETS = demo pidns mntns

.PHONY: all clean

all: $(TARGETS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGETS) *.o