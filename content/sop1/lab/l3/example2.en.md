---
title: "Laboratory task 3: threads, mutexes, signals"
date: 2022-02-07T20:02:06+01:00
bookHidden: true
---

## Task

Write a program which performs parallel operations on an array of
integers. Program takes `n` (`8 <= n <= 256`) and `p`
(`1 <= p <= 16`) as arguments. Then it creates an array of
subsequent integers from `0` to `n-1`. Parameter `p` is the number of
working threads running at the same time.

After receiving `SIGUSR1` program generates two random numbers `a` and
`b` (`0 <= a < b <= n-1`) and spawns a new thread to perform
inversion on array - swap elements `a` and `b`, `a+1` and `b-1`, `a+2`
and `b-2` etc. until {{< katex >}}a+\lfloor\frac{b-a-1}{2}\rfloor{{< /katex >}}. After each swap
thread, wait 5ms. A single swap should be atomic (you should make sure
that swapped elements are not modified by different threads) - use mutex
per array element.

After receiving `SIGUSR2`, the program spawns a new thread which prints
the state of the array. To print the array in a valid, existing state,
you should lock it whole (otherwise swapping threads can make changes
during printing).

If a new signal arrives when there are already `p` working threads, the
program should print message `All thread busy, aborting request` and
return to waiting. Working threads should be counted and monitored. When
one of the threads finishes its job, a new request can be processed.

Think about locking order to ensure operations safety and avoid
deadlocks. You don’t need to (and you shouldn’t) perform redundant
operations like swapping elements with itself.

You should not put heavy logic in signal handlers - only a few
assignments or simple arithmetic operations are permitted. Any system
functions are not permitted.

Stages:

## Graded stages

1.  Create array and mutexes as per task description. After receiving
    `SIGUSR1`, swaps are performed by the main thread.
2. Implement printing on `SIGUSR2` using new, detached thread.
3. Move swapping operation to a separate thread. Implement thread count limit logic.
4. After receiving `SIGINT` (ctrl+c) program joins all threads and cleanly terminates.
