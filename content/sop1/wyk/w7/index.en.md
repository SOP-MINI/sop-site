---
title: "Synchronization"
date: 2022-02-03T19:09:50+01:00
weight: 70
---

# POSIX synchronization

## Scope

  - POSIX semaphores
      - Persistence, naming, storage area for named and unnamed semaphores.
      - Creation and initialization, opening/closing access, removal of a named semaphore. `sem_open()`, `sem_close()`, `sem_unlink()`.
      - Creation and initialization, removal of an unnamed semaphore. Note: for inter-process synchronization semaphore structure (`sem_t`) instance has to be a part of a shared memory object.
      - Blocked and unblocked **wait** operations: `sem_wait()`, `sem_trywait()`.
      - **post** operation: `sem_post()`. Getting semaphore value: `sem_ getvalue()`
  - Other POSIX synchronization objects
      - Mutex. Properties. Basic pattern of use.
      - Condition variable. Use of condition variable-mutex pair for synchronization.
      - Barrier. The idea.
      - Multiple readers, single writer (read-write) locks.

## Reference

1.  Slides: [IPC\_2en.pdf]({{< resource "IPC_2en_4.pdf" >}})
2.  Section 27.1 of the GNU C library documentation: [Semaphores](https://www.gnu.org/software/libc/manual/html_node/Semaphores.html#Semaphores)
3.  Supplementary reading: W.R. Stevens, Unix Network Progamming, Vol. 2, 2nd Ed.  
    Chapters: 7 (Mutexes and Condition Variables), 8 (Read-Write Locks), 10 (Posix semaphores), 11 (System V Semaphores).

