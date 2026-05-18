---
title: "Slides"
type: presentation
layout: single
---

# Servers

Operating Systems 2 Lecture

<small>Warsaw University of Technology<br/>Faculty of Mathematics and Information Science</small>

---

### Iterative servers

The simplest possible model for a TCP server. It handles exactly one client at a time, 
within a single thread of execution, in a serial fashion.

```c
while (1) {
    int client_fd = accept(listen_fd, NULL, NULL);
    // Process client
    handle_client(client_fd); 
    // Close connection before taking the next one
    close(client_fd);
}
```

- **Pros**: Extremely simple, no synchronization needed
- **Cons**: Total lack of concurrency (not even mentioning parallelism here)

---

### The Blocking Problem

In an iterative server, the `handle_client()` function usually involves blocking I/O syscalls such as `read()`/`write()`.

If the client is slow, or simply stays connected without sending data, the entire server is blocked. Consider:

- long-lasting I/O bound requests (i.e. triggering large database queries)
- CPU intensive requests
- clients slowly receiving data (after buffers became full)
- clients slowly sending requests (_the slowloris attack_)

---

### Event multiplexers

The oldest, _classy_ multiplexing API is `select()`. It uses bitmasks to represent sets of file descriptors.

```c
fd_set read_fds;
FD_ZERO(&read_fds);
FD_SET(fd1, &read_fds);
FD_SET(fd2, &read_fds);
int n = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
```

Cons:
- **FD limit**: `FD_SETSIZE` is usually `1024`. 
- **Efficiency O(N)**: Every time select returns, the app must loop through all FDs to find which one is ready.
- **Stateless**: The kernel doesn't remember the set; you must rebuild the bitmask every single time.

---

### The poll() syscall

`poll()` was designed to solve some `select()` issues.

```c
struct pollfd fds[10000];
fds[0].fd = fd1; fds[0].events = POLLIN;
// ...
int ret = poll(fds, 10000, -1);
```

- **Improvement:** No hard limit of 1024 FDs.
- **O(N) Remains:** The kernel still has to scan the whole list, and the application still has to iterate through the whole array to find revents != 0.

---

### The epoll() syscall

The modern Linux solution. It is **stateful** and **event-triggered**.

Epoll instance is an OS-level object consisting of two lists:
1. The **Interest List** (Monitored FDs) (Red-Black Tree)
2. The **Ready List** (Triggered FDs) (Doubly Linked List)

Application creates the epoll and manipulates it via a set of dedicated syscalls:

- `epoll_create()`: Create an epoll instance
- `epoll_ctl()`: Add/Remove/Modify FDs to the interest list
- `epoll_wait()`: Consume events from the ready list (blocking)

When `epoll_wait()` is called, the kernel doesn't search anything. It checks if the ready list is empty.
If not, it copies its contents directly to user-space.

---

### Event representation

`epoll_ctl` expects a event description along with monitored file descriptor.

```c
typedef union epoll_data {
    void *ptr; int fd; uint32_t u32; uint64_t u64;
} epoll_data_t;

struct epoll_event {
    uint32_t     events; /* Event bitmask */
    epoll_data_t data;   /* User data (context) */
};
```

`events` field determines set of awaited event types.

`data` is arbitrary, application specific context
passed within returned events. Simple applications just use `fd` to read back the descriptor. 
`ptr` field is often used to retrieve arbitrary context (containing `fd`).

---

### Event types

| Flag             | Description                          
|:-----------------|:-------------------------------------|
| `EPOLLIN`        | Read-ready                           |
| `EPOLLOUT`       | Write-ready                          |
| `EPOLLRDHUP`     | Peer closed connection               |
| `EPOLLPRI`       | Priority (OOB) data arrived          |
| `EPOLLERR`       | Error occured (i.e. `RST`) [*]       |
| `EPOLLHUP`       | Abnormal termination [*]             |

Application can combine events: `EPOLLIN | EPOLLOUT` and receive more than one event in a single structure.

Events maked with `[*]` are monitored automatically.

---

### Event modifiers

Application may provide auxiliary flags to `epoll_ctl()` telling the OS
when where and how to deliver the events.

#### `EPOLLET`
Changes behavior from the default _level-triggered_ to _edge-triggered_.
Event will be generated only when state changes (normally `wait()` checks state on call).

#### `EPOLLONESHOT`
Automatically disarms the descriptor when it moves to a ready list. 
No more events will be generated until re-armed with `EPOLL_CTL_MOD`.

#### `EPOLLEXCLUSIVE`
In case there are multiple callers of `epoll_wait()` wakes just a single one.

---

### Thread-Per-Connection

The old approach to handling multiple clients. To solve the iterative blocking, we can spawn a new thread for every `accept()`.

```c
while (1) {
    int *client_fd = malloc(sizeof(int));
    *client_fd = accept(listen_fd, NULL, NULL);
    pthread_t tid;
    pthread_create(&tid, NULL, thread_worker, client_fd);
    pthread_detach(tid);
}
```

- Native concurrency - one client doesn't block others.
- Simple, synchronous, blocking code
- Significant undercontrolled threading overhead

---

### Limits of Multi-threading

Is spawning 10k threads when there are 10k clients a good idea?

- **Memory:** Each thread requires a stack (typically 2MB-8MB). 10k threads equals ~20GB-80GB of RAM **just** for stacks.
- **Context Switching:** The OS Scheduler must juggle thousands of threads. The CPU spends more time switching tasks (saving/restoring registers) than doing actual work.

Those are the defining factors of **The C10k Problem:** A term coined in 1999. Can a single server handle 10,000 concurrent connections?

---

### Event-driven servers

Because of the C10k problem, the world has moved from blocking I/O to event-driven architectures based around `epoll()`.

Instead of "One thread per client", we want "One thread for many clients".

![kworker.svg](/ops2/wyk/servers/epoll_flow.svg)

Using `epoll()` removes both problems: memory and context switching overheads disappear:
- _ctx switches_ between clients are managed entirely within the application, without kernel intervention.
- single stack is used for all the clients, switching between them

---

### Scaling out epoll

Even with the `epoll`, a single thread can be overwhelmed by high traffic.
We need to take advantage of hardware paralleism and **use more cores!**.

`epoll_wait()` is safely callable from within multiple concurrent threads.
Using it correctly, is incredibly tricky. Naive usage suffers from **_Thundering Herd Problem_**.
Once a descriptor becomes ready for I/O, kernel is going to wake up **all**
the threads unnecessarily, wasting CPU time. In case of client sockets
we also risk **race condition**: two threads reading same socket.

![kworker.svg](/ops2/wyk/servers/thundering_herd.svg)

---

### Solution 1: Shared epoll

Use `EPOLLET | EPOLLONESHOT`. Edge-triggered mode delivers events only when changes occur
on the monitored file descriptor. Each change is delivered to a single thread chosen by the kernel.

One-shot causes the descriptor to be automatically disarmed. 
It is necessary to avoid splitting chunks of the same stream between multiple threads (race condition).
After I/O is done, the chosen thread must re-arm it with `EPOLL_CTL_MOD`.

This scheme works well for connected client sockets. Each time client sends, one of the threads
is assigned to it exclusively, re-arming at the end.

It does not scale out `accept()` at all. No other thread may accept while the socket is disarmed.

---

### Solution 2: Private epolls

Solution 2: Have independent epoll instances in each worker. Dedicated acceptor thread
assigns accepted clients to one of workers in a round-robin fashion via thread-safe `epoll_ctl_add()` into worker's epoll instance.

![kworker.svg](/ops2/wyk/servers/private_epolls.svg)

---

### Scaling out accept

Accepting thousands incoming connections per second is more tricky.
Single accepting socket with its backlog is a bottleneck.

Modern `SO_REUSEPORT` socket option allows for having multiple listening sockets
bound on the same port. Each thread may have its own accept backlog for the same port.

![kworker.svg](/ops2/wyk/servers/so_reuseport.svg)

---

### Event loop structure

While `epoll` solves performance problems, it creates an engineering nightmare.

Epoll based event loop is a large monolithic routine with giant `if`/`switch` block
choosing right event handling code based on the incoming event.

```c
int n = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
for (int i = 0; i < n; i++) {
    int fd = events[i].data.fd;
    if (fd == listener_fd) {
        // accept
    } else if (events[i].events & EPOLLIN) {
        // read & process
    }
}
```

---

### The Reactor Pattern

A design pattern appeared based around the **Inversion of Control** principle.

1. **Event Demultiplexer:** The OS-level mechanism that waits for events (`epoll`).
2. **Reactor / Dispatcher:** The infinite Event Loop. It calls `epoll_wait()`, retrieves triggered events, and routes them.
3. **Handlers:** Application-level state machines. They perform the actual non-blocking I/O (`read`, `write`, `accept`).

```c
int n = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
for (int i = 0; i < n; i++) {
    handler_t *item = events[i].data.ptr;
    if (events[i].events & EPOLLIN && item->on_read) {
        item->on_read(item); // Polymorphic call
    }
    if (events[i].events & EPOLLOUT && item->on_write) {
        item->on_write(item); // Polymorphic call
    }
}
```

---

### The Reactor Pattern

![kworker.svg](/ops2/wyk/servers/reactor.svg)

---

### State Machines Problem

Single thread multiplexes several independent client flows.

![kworker.svg](/ops2/wyk/servers/client_flows.svg)

This is explicitly implemented with **State Machine** pattern:

```c
void on_read(handler *h) {
 switch (h->ctx->state) {
    case READ_INIT:
        // ...
    case READ_REQUEST:
        // ...
    }
}
```

---

### Suspendable functions

The problem state machine solves is the inability to **suspend a function** in places where I/O must occur.

```c
void handle_client(int fd) {
    while (true) {
        read(fd, request); // and yield to others
        // process
        write(fd, response); // and yield to others
    }
} 
```

Languages expose explicit feature solving this problem much more explicitly: **coroutines**.

---

### Coroutines

In contrast to _"normal"_ functions which are called and do return,
coroutines are **created**, can be **resumed**, may **suspend** and finally do **exit**.  

```cpp
Task handle_client(ClientSocket s) {
  char buf[1024];
  while (true) {
      ssize_t n = co_await s.AsyncRead(buf, sizeof(buf));
      if (n <= 0) break;
      // ...
  }
}
```

Where `co_await operator` is used, the function is **suspended** and returns to the caller/resumer.
Note that locals along with coroutine state cannot be maintained on the thread's stack which is then used by
other coroutines or regular code during suspension. **Coroutine frames** are stored on the heap.

---

### Zero-Copy solutions

Servers tend to spend lots of time just copying buffers to/from the kernel space:

`NIC` ➔ `Rx Buffer` ➔ `User Space (char buf[])` ➔ `Tx Buffer` ➔ `NIC`.

`io_uring` eliminates the double copy problem by sharing a single buffer between
the kernel and userspace. It works nicely with the `epoll()` infrastructure.

There are also syscalls designed to solve the copying overhead by moving data between
two file descriptors without copying between kernel and user spaces: `sendfile()`, `splice()`.