---
title: "Synchro Patterns"
date: 2022-02-10T10:00:00+01:00
weight: 30
bookCollapseSection: true
---

# Synchronization Patterns

## Scope

* POSIX Semaphores
   * Basic API revisited
   * Placing in shared memory
   * Awaiting others
   * Named semaphore implementation (small shm)
* Bounded buffer problem
   * Semaphore solution
* Readers-Writers problem
   * Solution favorizing readers
   * Solution prioritizing writers
* PThread Mutex
   * Basic API revisited
   * Placing in shared memory
   * Error checking mutexes
   * Recursive mutexes
   * Robust mutexes
* PThread CV
   * Basic API revisited
   * Placing in shared memory
   * Awaiting others
* Bounded buffer problem solution using CV
* Monitors
* PThread reader-writer lock

## Reference

1. [Lecture Slides](slides)
2. [Code samples](code)
3. Slides: [IPC\_2en.pdf]({{< resource "IPC_2en_4.pdf" >}})
4. Section 27.1 of the GNU C library
   documentation: [Semaphores](https://www.gnu.org/software/libc/manual/html_node/Semaphores.html#Semaphores)
5. Supplementary reading: W.R. Stevens, Unix Network Progamming, Vol. 2, 2nd Ed.  
   Chapters: 7 (Mutexes and Condition Variables), 8 (Read-Write Locks), 10 (Posix semaphores), 11 (System V Semaphores).

