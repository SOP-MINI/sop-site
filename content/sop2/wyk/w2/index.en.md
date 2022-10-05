---
title: "Lec2. POSIX IPC: shared memory and message queues"
date: 2022-02-10T10:00:00+01:00
---

## Scope

  - Basic information about POSIX interfaces to IPC.
      - Three types of IPC objects: message queues, shared memory segments, semaphores.
      - Two families of interface functions: `IPC UNIX System V`, `POSIX IPC`
      - IPC object persistence: kernel, processs, file system.
      - Namespaces, IPC object visibility (e.g. in the file system)
  - `POSIX IPC` interface to message queues and shared memory:
      - Namespace, persistence, visibility.
      - Message, message priority, message passing (limited buffering, retrieval: the oldest message of the highest priority - first), asynchronous notification, message queue removal and its consequences.
      - Shared memory segment: creation, attaching/detaching, access modes, access violations, marking segment for remvoal vs. removal from the system.
      - Calls of `fork()`, `exec()`, `_exit()` functions and IPC objects.
      - (\*\*) Interface design and example use
          - Visibility of IPC objects in the file system
          - Interface functions related to message queues (`mq_*()`) and shared memory segments (`shm_*()`, `ftruncate()`, `mmap()` etc.); selected data structures of the interface.
          - Asynchronous notification when a new message arrives at the empty message queue.
  - Message queues - `UNIX System V IPC` interface
      - Namespace, persistence, visibility.
      - Message, message type, message passing (limited buffering, selective retrieval: FIFO order, by type), message queue removal and its consequences.
      - Shared memory segment: creation, attaching/detaching, access modes, access violations, marking segment for remvoal vs. removal from the system.
      - (\*) Interface design and example use
          - Visibility of IPC objects in the system; system commands (`ipcs`, `ipcrm`)
          - Interface functions for message queues (`msg*()`) and shared memory segments (`shm*()`); selected data structures of the interface.

### Note

  - (\*) mark denotes auxiliary information, which is helpful in understanding the main (i.e. remaining) content. Thus detailed knowledge (e.g. about `UNIX System V IPC` interface function calls and related system data structures) is not required during lecture tests.
  - (\*\*) mark denotes content, which might be of direct use for laboratory work, and also might be beneficial for better understanding of general concepts. However, detailed knowledge of the interface function calls is not required during lecture tests.
  - During lecture tests knowledge and understanding of general concepts and interface properties are expected.

## Reference

1.  Slides: [IPC\_1en.pdf]({{< resource "IPC_1en_3.pdf" >}}),
2.  Supplementary reading: W.R. Stevens, Unix Network Progamming, Vol. 2, 2nd Ed.: Interprocess Communications,
    Chapters: 1 (Introduction), 2 (Posix IPC), 3 (System V IPC), 5 (Posix Message Queues), 6 (System V Message Queues), 12 (Shared Memory Introduction), 13 (Posix Shared Memory), 14 (System V Shared Memory)
