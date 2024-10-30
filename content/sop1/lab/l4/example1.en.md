---
title: "Laboratory task 4: synchronization"
date: 2022-02-07T20:02:06+01:00
bookHidden: true
---

## Task

Write a program that takes 3 input parameters: n,m,path. where n is the number of processing posix threads and path is a csv file path. Main thread reads the header of the file, and checks the size of the file. It divides the rest of the file equally into m chunks. Main thread creates thread pool of n threads. Each processing thread is then reading next chunk that is available at time and saving read lines into linked list. If the thread encounters an error during reading the file (missing comma or to many commas) it waits all until all currently running worker threads finish reading, reports to the stderr error including line number in the file it was encountered, stops further processing and then program exits. When all file content has been read, the resulting linked lists are joined into one linked list and printed by one thread to stdout.

## Graded stages

At each stage run the program with n = 3, m = 10, and file=example.csv when presenting the results

1. Main thread reads csv header and file size and divides it into m chunks. It creates n threads. Each thread is printing "*" and exits. (3p.)
2. Implemented thread pool. Threads take one available chunk task and prints assigned chunk(start and size in bytes) until there is no chunk tasks left. Main thread exits when there is no more tasks. (4p)
3. Processing threads are all reading assigned chunks and saving read lines into linked lists. (3p)
4. When thread encounters an error it waits for all currently running threads, checks the line number of the error and prints it to stderr. The processing stops and the program exits. (4p)
5. One thread is concatenating resulting linked lists and prints it line by line to stdout. (2p)
