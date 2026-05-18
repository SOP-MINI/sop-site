CC = gcc
CXX = g++
CFLAGS = -Wall -Wextra
CXXFLAGS = -Wall -Wextra -O2 -std=c++20
LDFLAGS = -lrt

DUMB_TARGETS = dumb_iterative dumb_multithreaded dumb_epoll dumb_epoll_workers dumb_reactor
CHAT_TARGETS = select_chat epoll_chat
CORO_TARGETS = dumb_coro

TARGETS = $(DUMB_TARGETS) $(CHAT_TARGETS) $(CORO_TARGETS)

.PHONY: all clean

all: $(TARGETS)

$(DUMB_TARGETS): %: %.c dumb_common.c dumb_common.h
	$(CC) $(CFLAGS) $< dumb_common.c -o $@ $(LDFLAGS)

$(CHAT_TARGETS): %: %.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

$(CORO_TARGETS): %: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGETS) *.o