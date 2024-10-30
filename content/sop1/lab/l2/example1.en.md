---
title: "Graded task on Processes, Signals and Descriptors"
date: 2022-02-07T20:02:02+01:00
bookHidden: true
---

## The problem:

Write a program that creates “n” children subprocesses, where “n” is sole program parameter. Each child process has to generate random number of seconds “s” form the range of \[100-200\] milliseconds. Child process in a loop sleeps “s” milliseconds (1000 milliseconds = 1 second) and sends SIGUSR1 to its parent. Child process gets terminated by SIGUSR2 signal. Parent process counts the number of signals received and once it reaches 100 it terminates its children, waits for them to finish and exits.

## Graded stages:

1.  Program creates “n” children, each child process chooses random “s” and prints it on the screen. Parent can exit prior to its children
2.  Child process sends 30 times SIGUSR1 to its parent in a loop with “s” delay, print one “\*” for each signal sent (4p)
3.  Parent process counts the SIGUSER1 and prints counter every time it receives the signal. At this stage it must be terminated with C-c. (4p)
4.  When counter in parent process reaches 100 terminate the child processes and exit, child process are modified to send SIGUSR1 until they receive SIGUSR2 (3p)
5.  Wait for child processes at the end of parent and eliminate possible zombies during the operation (SIGCHLD)(3p)
