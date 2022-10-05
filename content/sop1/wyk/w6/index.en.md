---
title: "Threads"
date: 2022-02-05T22:35:46+01:00
weight: 60
---

# Lecture 6 - Threads, P-threads and mutexes

## Scope

  - Thread concept. Single- and multiple-thread processes.
  - Advantages of multi-threading. Scaling and Amdahl's law.
  - Multicore programming issues.
  - User and kernel threads
  - Multithreading models. Co-routines; fibers.
  - Thread libraries: POSIX threads, Windows threads
  - Implicit threading. Thread pools. OpenMP
  - Threading issues:fork/exec, signal handling, cancellation, thread-local storage, scheduler activations
  - POSIX threads API:
      - thread creation (`pthread_create()`) and ending (`pthread_exit(), exit(), pthread_cancel()`, returning from/exiting `main()` function)
      - thread attributes
      - passing parameters to thread function; returning result
      - waiting for joinable thread (`pthread_join()`), detaching thread (`pthread_detach`)
      - synchronization: motivation, using `mutex` to protect critical section related to some shared, concurrently accessed data
      - re-entrant and thread safe functions
      - threads and signals: thread signal masks and their use

## Reference

1.  Textbook: chapter 5 "Threads" - in 8th and 9th ed., or chapter 4 "Threads & Concurrency" - in 10th ed.
2.  Slides: [Threads.pdf]({{< resource "Threads_3.pdf" >}}), [POSIX\_threads.pdf]({{< resource "POSIX_threads_9.pdf" >}})
3.  Extra reading: The GNU C library documentation: [1.2.2.1 POSIX Safety Concepts](https://www.gnu.org/software/libc/manual/html_node/POSIX-Safety-Concepts.html#POSIX-Safety-Concepts), [12.5 Streams and Threads](https://www.gnu.org/software/libc/manual/html_node/Streams-and-Threads.html#Streams-and-Threads)
4.  Exemplary lecture code: [pth1.c]({{< resource "pth1.c" >}}), [pth2.c]({{< resource "pth2.c" >}}), [pth3.c]({{< resource "pth3.c" >}}), [sighand.c]({{< resource "sighand.c" >}})
5.  [Excerpt]({{< resource "IEEE_Std_1003.1_2008_Thread_cancellation.pdf" >}}) from the POSIX Standard on threads cancellation
