---
title: "Network programming"
date: 2022-02-03T19:24:24+01:00
weight: 70
---

# Lecture 7 - Network programing

## Scope
  
### Basic content

  - Client-server model.
      - Concurrent and iterative server types. Communication type vs server type.
      - Stateful services. Stateful and stateless servers.
  - Peer to peer (P2P) models. Pros and coms of P2P as compared to the client-server model.
  - Service quality criteria.
  - Handling multiple communication channels
      - blocking I/O + threads or sub-processes.
      - Synchronous I/O multiplexing - `select()`, `pselect()`

### Auxiliary content

  - Non-blocking I/O.
  - SIGIO and I/O
  - Running servers

## Reference

1.  Slides: [Inet\_3en.pdf]({{< resource "Inet_3en_4.pdf" >}})
2.  *The GNU C library* documentation: 13.8: [Waiting for Input or Output](http://www.gnu.org/software/libc/manual/html_node/Waiting-for-I_002fO.html#Waiting-for-I_002fO), 16.11: [The inet daemon](http://www.gnu.org/software/libc/manual/html_node/Inetd.html#Inetd), 18.1 [Overview of Syslog](http://www.gnu.org/software/libc/manual/html_node/Overview-of-Syslog.html#Overview-of-Syslog), 24.2.4: [Asynchronous I/O Signals](http://www.gnu.org/software/libc/manual/html_node/Asynchronous-I_002fO-Signals.html#Asynchronous-I_002fO-Signals)
