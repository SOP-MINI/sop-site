---
title: "Event I/O"
date: 2025-11-18T22:35:27+01:00
weight: 55
---

# Event I/O

## Scope

- IO and CPU bound applications
- Blocking syscalls problem
- Busy waiting
- Event-based programming
  - Receiving events via signals
  - Receiving events via blocking syscalls
- Event based APIs:
  - `timer_create`
  - `timerfd_create`
  - `signalfd`
- Change detection
  - ABA problem
- Filesystem events
  - `inotify` API
- Asynchronous I/O
  - `io_uring` API (note: advanced, auxiliary topic)
- Event multiplexing
  - `select()` API
  - event loops

## Reference

1. [Lecture presentation](https://link.excalidraw.com/p/readonly/iDk9bAu6v23YGNE9iXHs) ([PDF]({{< resource OPS1_EventIO.pdf >}}))
2. [Lecture code samples](https://gitlab.com/SaQQ/sop1/-/tree/master/05_events?ref_type=heads)
