---
title: "Laboratory task 3: threads, mutexes, signals"
date: 2022-02-07T20:02:06+01:00
bookHidden: true
---

## Task

Write a program that takes 2 input parameters: n,k. where n is the number of processing posix threads and k is the task array size. The main thread fills the task array with random floating point number of range \[1,60\]. Main thread creates n processing threads. Each processing thread is calculating the square root of random array cell and stores the result in result array, prints: “sqrt(a) = b” (b is a result, a is an input) and then sleeps for 100ms. The programmer must ensure that each cell is calculated only once and each calculation should be mutex protected. The program ends when all cells are calculated. The main thread is waiting for all threads and then prints input and results arrays.

## Graded stages

At each stage run the program with n=3 and k = 7 when presenting the results

1.  Creating n+1 threads. Each thread is printing “\*”. Main thread is waiting for all threads. (4p)
2.  Main thread is filling the task array and prints it, the processing threads are choosing random array cell number, print its index and exit. (4p)
3.  Processing threads are calculating result of one random cell with mutex synchronization, print the result and exit . Main thread is printing both arrays afterwards (after the join). At this stage cells can be calculated more than once but not in parallel. (5p)
4.  Each cell is protected by separate mutex. Processing threads are counting remaining cells to compute. If that number reaches 0, threads are terminating otherwise they calculate the next random remaining cell and sleep (4p)
