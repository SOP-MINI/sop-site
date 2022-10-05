---
title: "List of topics for Lab3 introductory test (threads)"
date: 2022-02-07T20:06:46+01:00
bookHidden: true
---

Only the new topics. You still need all previous labs knowledge which is not listed here.

## Essential concepts

  - What is the difference in error reporting between pthread functions and typical POSIX functions covered in previous labs?
  - Which thread interactions (creating, joining, canceling, sending signals, etc.) are possible between threads in different processes in the systems and which are limited to threads within a single process?
  - How does the fork call work in a multithreaded program?
  - Thread cancellation:
      - What types of cancellation are available in POSIX. How do they differ from each other?
      - Why do we need cleanup handlers?
      - How do cleanup handlers work?
      - What does the argument of `pthread_cleanup_pop` mean?
      - Which functions defined in POSIX can be a thread cancellation points?
  - Mutexes
      - Why do we need them?
      - How to create, lock, unlock, destroy a mutex?
      - What are reentrant mutexes?
      - Which threads can unlock a mutex locked by a specific thread?
  - Basic operations: creation, joining, destroying threads
      - How to set thread attributes?
  - What are detached threads and how to make a thread detached?
      - Which operations can’t be performed on a detached thread?
  - Signals
      - Which threads can hadnle a signal delivered to a process?
      - If there are multiple threads that can handle a signal how many of them will handle it?
      - What is the difference between synchronous and asynchronous signal handling?
      - Setting signal masks.
  - Which of `pthread_` functions can be interrupted by async signal handlers?
  - Library functions that are not thread safe
      - Which library functions cannot be used safely from multiple threads and why?
      - Thread-safe variants of unsafe functions
  - `errno` in multithreaded programs

## Important functions and macros

  - `pthread_join`
  - `pthread_detach`
  - `pthread_exit`
  - `pthread_create`
  - `pthread_sigmask`
  - `pthread_mutex_init`
  - `pthread_mutex_destroy`
  - `pthread_mutex_lock`
  - `pthread_mutex_trylock`
  - `pthread_mutex_unlock`
  - `pthread_cleanup_push`
  - `pthread_cleanup_pop`
  - `pthread_cancel`
  - `pthread_testcancel`
  - `pthread_attr_init`
  - `pthread_attr_setdetachstate`
  - `PTHREAD_MUTEX_INITIALIZER`
  - `sigwait`
  - `rand_r`
  - `strtok_r`
