---
title: "POSIX synchronization"
date: 2022-02-03T19:09:50+01:00
weight: 40
---

# Lecture 4 - POSIX synchronization

## Scope
  

  - UNIX System V semaphore sets
      - Persistency, namespace.
      - Original concepts: semaphore set, extended semaphore operations (incrementation and decrementation by an integer value, waiting for zero value, atomic execution of a set of operations, undoing specific operations before process termination).
      - Basic uses of extended semaphore feautures
      - (\*) Interface design
          - Data related to each semaphore: `semval, sempid,          semncnt, semzcnt, semadj`. Important data structures: `struct sembuf`, `union semun`.
          - Creation and initialization, opening, removal of a semaphore set: `semget()`, `semctl()`.
          - Basic and extended semaphore operations: `semop()` function.
  - (\*\*) POSIX semaphores
      - Persistence, naming, storage area for named and unnamed semaphores.
      - Creation and initialization, opening/closing access, removal of a named semaphore. `sem_open()`, `sem_close()`, `sem_unlink()`.
      - Creation and initialization, removal of an unnamed semaphore. Note: for inter-process synchronization semaphore structure (`sem_t`) instance has to be a part of a shared memory object.
      - Blocked and unblocked **wait** operations: `sem_wait()`, `sem_trywait()`.
      - **post** operation: `sem_post()`. Getting semaphore value: `sem_ getvalue()`
  - Other POSIX synchronization objects
      - (\*\*) Mutex. Properties. Basic pattern of use.
      - (\*\*) Condition variable. Use of condition variable-mutex pair for synchronization.
      - Barrier. The idea.
      - Multiple readers, single writer (read-write) locks.

### Note

  - (\*) mark denotes auxiliary information, which is helpful in understanding the main (i.e. remaining) content. Thus detailed knowledge (e.g. about `UNIX System V IPC` interface function calls and related system data structures) is not required during lecture tests.
  - (\*\*) mark denotes content, which might be of direct use for laboratory work, and also might be beneficial for better understanding of general concepts. However, detailed knowledge of the interface function calls is not required during lecture tests.
  - During lecture tests knowledge and understanding of general concepts and interface properties are expected.

## Reference

1.  Slides: [IPC\_2en.pdf]({{< resource "IPC_2en_4.pdf" >}})
2.  Section 27.1 of the GNU C library documentation: [Semaphores](https://www.gnu.org/software/libc/manual/html_node/Semaphores.html#Semaphores)
3.  Supplementary reading: W.R. Stevens, Unix Network Progamming, Vol. 2, 2nd Ed.  
    Chapters: 7 (Mutexes and Condition Variables), 8 (Read-Write Locks), 10 (Posix semaphores), 11 (System V Semaphores).

