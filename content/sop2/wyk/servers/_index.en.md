---
title: "Servers"
weight: 70
bookCollapseSection: true
---

# Servers

## Scope

- Iterative servers
- Event demultiplexers: `select()`/`poll()`/`epoll()`
- _Thread-per-connection_ architecture
- C10k problem
- Quality metrics: QPS, EPS, latency
- Event loop based servers
- Scaling out event loops
    - shared demultiplexers with `EPOLLONESHOT`
    - private demultiplexers
    - `SO_REUSEPORT` to scale out `accept()`
- _Reactor_ pattern
- _State Machine_ pattern
- Coroutines
- _Zero-Copy_ solutions

## Reference

1. [Lecture Slides](slides)
2. [Code samples](code)
3. Old slides: [Inet\_3en.pdf]({{< resource "Inet_3en_4.pdf" >}})
4. *The GNU C library* documentation:
   13.8: [Waiting for Input or Output](http://www.gnu.org/software/libc/manual/html_node/Waiting-for-I_002fO.html#Waiting-for-I_002fO),
   16.11: [The inet daemon](http://www.gnu.org/software/libc/manual/html_node/Inetd.html#Inetd),
   18.1 [Overview of Syslog](http://www.gnu.org/software/libc/manual/html_node/Overview-of-Syslog.html#Overview-of-Syslog),
   24.2.4: [Asynchronous I/O Signals](http://www.gnu.org/software/libc/manual/html_node/Asynchronous-I_002fO-Signals.html#Asynchronous-I_002fO-Signals)
