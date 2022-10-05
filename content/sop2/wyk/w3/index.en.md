---
title: "Lec3. Synchronization. Deadlocks"
date: 2022-01-28T17:23:23+01:00
---

## Scope
  
### Basic content

  - Synchronization.
      - Introduction - naive solution of the producer-consumer problem; diagnosis; race condition
      - The critical section problem and 3 conditions for correct solution
      - Synchronization hardware. `swap()` and `test_and_set()` functions. Bounded-waiting Mutual Exclusion with `test_and_set()`
      - Semaphores: binary and counting. Implementation. Use. Limitations: deadlock, starvation, priority inversion. Basic error of use.
      - Classical Problems of Synchronization: Bounded-Buffer Problem, Readers and Writers Problem, Dining-Philosophers Problem.
      - Monitor and condition variable concepts
      - Atomic transactions
  - Deadlocks.
      - Deadlock problem. Four conditions of deadlock.
      - Resource allocation graph and the deadlock.
      - Three methods for handling deadlocks: Deadlock prevention or avoidance, Detection and recovery, Ignoring.
      - Safe state, safe sequence, banker algorithm
      - Deadlock detection

### Auxiliary content

  - Synchronization examples: Solaris, Windows, Linux, Pthreads.
  - Concurrent transactions

## Reference

1.  Textbook: chapters: Process Synchronization, Deadlocks.
2.  Slides: [Sync.pdf]({{< resource "Sync_2.pdf" >}}), [Deadlocks.pdf]({{< resource "Deadlocks_0.pdf" >}})
