---
title: "Task on shared memory and mmap"
bookHidden: true
math: true
---

## Shared Integration

The goal of this task is to write a program for numerical integration using the Monte Carlo method for the function:

{{< katex >}}\int_a^b{e^{-x^2}dx}{{< /katex >}}

The program should utilize shared memory to enable multiple processes to cooperate.  
Each newly launched program should join the calculations and speed them up.

Each process maintains its own private computation.  
After evaluating `N` randomly sampled points, the program should update the counters for total samples and successful hits (`batch processing`).

To coordinate calculations in shared memory, the program must store not only the computed result but also a counter of cooperating processes.  
At startup, each process should increase this counter and decrease it upon proper termination.  
When a process decreases the counter to zero, it should summarize the calculation results and display the final approximation on the screen.

## Stages

1. Use a named shared memory object for inter-process cooperation.  
   Prepare a shared memory structure containing a process counter protected by a shared mutex.

   Write a procedure for initializing shared memory with correct counter incrementation when new processes start.  
   To prevent race conditions between shared memory creation and initialization, use a named semaphore.

   After properly initializing shared memory, the process should print the number of collaborating processes, sleep for 2 seconds, and then terminate.  
   Before a process exits, it should destroy the shared memory object if it is the last process to detach.

2. Implement three batches of `N` Monte Carlo sample evaluations.  
   Accept program parameters as described in the `usage` function.  
   Use the provided `randomize_points` function to compute one batch of samples.

   Extend the shared memory structure with two counters describing the number of total samples and successful hits.  
   After computing each batch, update the counters and print their status to standard output.  
   After completing three computation iterations, the program should terminate, following the shared memory destruction logic from Stage 1.

   > **It is advisable to store the integration limits in shared memory to avoid a process joining with different integration bounds. Such a scenario would cause approximation results to be meaningless.**

3. Add handling for the `SIGINT` signal, which interrupts further batch computation.

   In this stage, the program should continue approximating the integral until it receives the signal.  
   A sufficiently good implementation is to finish computing the current batch and skip taking another one.

   If the process is the last to detach from shared memory, it should print the result to standard output.

4. Add handling for process termination while holding a mutex in shared memory outside of the initialization procedure.  
   Change mutexes to be robust and handle the situation when the owner process dies.  
   Upon detecting such a situation, assume that the process counter should be decremented to ensure proper finalization of the program.

   To simulate an abrupt process termination, use the `random_death_lock` function, which should be called when locking any mutex outside shared memory initialization.
  
## Starting code

{{< includecode "example2-code.c" >}}