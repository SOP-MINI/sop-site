---
title: "Synchronization"
date: 2022-02-10T10:00:00+01:00
weight: 20
bookCollapseSection: true
---

# Synchronization

## Scope

- The Critical Section problem
- Three correctness conditions: Mutual Exclusion, Progress, Bounded Waiting
- Peterson's Algorithm
- Memory view inconsistency (Cache, Store Buffers)
- Hardware instruction reordering
- Hardware memory models
- Impact of the compilation process
- C11+ memory model
- Memory barriers
- Hardware synchronization support: Read-Modify-Write operations
- Atomic Swap (XCHG)
- Test-and-Set (TAS)
- Compare-and-Swap (CAS / cmpxchg)
- Spinlocks and the busy-waiting problem
- Dijkstra's system semaphores
- Linux Futex (Fast Userspace Mutex)

## Reference

1. [Lecture Slides](slides)
2. [Code samples](code)
3. Textbook: chapters: Process Synchronization, Deadlocks.
4. Old slides: [Sync_2.pdf]({{< resource "Sync_2.pdf" >}}) 
