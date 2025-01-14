---
title: "Laboratory task 4: synchronization"
bookHidden: true
---

# Description

Your task is to prepare a pool of threads waiting for functions to
execute. Unlike creating threads on demand, a thread pool allocates
worker threads at the beginning of program execution, and they wait for
a signal to perform work. Then, from other threads, you can pass a
function to be executed by one thread from the pool.

In this lab, the thread pool should implement three functions:

1.  `initialize` -- create `N` threads within the thread pool.

2.  `dispatch` -- pass a pointer to a function with the signature
    `void (work_function)(void)` and the calling arguments to a thread
    waiting for work. If there is no free thread to perform the work,
    the `dispatch` function should block until a free thread is found.

3.  `cleanup` -- the function should wait for all worker threads to
    finish and perform thread `join`. It is not allowed to interrupt the
    work of a thread that has taken the task to execute a function.

The loop of a worker thread can be described in the following steps:

1.  Wait on a conditional variable for work assigned from another thread
    (one conditional variable shared between all worker threads).

2.  After taking over the work function and execution arguments, inform
    that it has happened. Otherwise, a race condition could occur,
    leading to unstable thread pool operation.

3.  Call the function with the received arguments.

4.  Repeat until the `cleanup` function is called on the thread pool.

To use the thread pool, a Monte Carlo method for calculating the area of
a circle has been implemented. The procedure consists of two parts:
drawing points and averaging the obtained results. The implementation
using our thread pool realizes these two parts as two different worker
functions: `circle_monte_carlo` and `accumulate_monte_carlo`. The
`accumulate_monte_carlo` function must start averaging the results only
after all `circle_monte_carlo` functions have finished. Your task in
this lab is to synchronize these two functions (a barrier is ideal for
this purpose) and provide them with the correct data.

For easier testing, a CLI interface has been implemented within the
starter code:
```
enter command
1. circle <n> <r> <s>
2. hello <n>
3. exit
```

It allows selecting a command with the corresponding number and passing
arguments separated by spaces. The first command runs the procedure to
calculate the area of a circle with a radius of `r` using `n` threads in
total, randomizing `s` samples in total. The second command runs `n`
test functions printing `Hello world from worker n!`. Choosing the third
option closes the program, waiting for all requested commands to finish.

**Hint**: Critical places in the code where something needs to be added
in each stage are marked with the comment `TODO STAGE-X`.

# Stages:

- Implement the `initialize` function. Created threads should print their
`TID` and wait on a conditional variable. 
- Implement the `dispatch`
function. The `hello` option should work at this stage. 
- Implement
passing data to calculate the area of a circle and synchronization with
the threads drawing points. The `circle` option should work at this
stage. 
- Implement the `cleanup` function. The program should wait for the
calculations to finish and correctly terminate all threads when choosing
the `exit` option.

# Starting code

- [sop1l4e3.zip](/files/sop1l4e3.zip)
