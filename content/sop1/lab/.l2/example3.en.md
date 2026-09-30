---
title: "Graded task no. 2 on Processes, Signals and Descriptors"
date: 2022-02-05T19:34:07+01:00
bookHidden: true
---

## Task
Write a program simulating a computational cluster.

The parent process creates N children, where N is the only command-line argument accepted by the program.
Each child is assigned a number between 0 and N-1.

The parent instructs the first child to do work by sending SIGUSR1 to it.
After receiving the signal, the child performs its work in a loop: it sleeps for a random amount of time from the range 100-200ms, and adds 1 to a local counter and prints {PID}: {counter}\n to stdout.

If the parent receives SIGUSR1, it sents SIGUSR2 to the currently working child and SIGUSR1 to the next child, i.e. the one with its index larger by one (for the last child, the next one should be the child with index 0).
The child which receives SIGUSR2 stops its loop and waits for SIGUSR1 to resume its work.
In short, SIGUSR1 instructs a child to begin its work, and SIGUSR2 to stop it.

After receiving SIGINT, the parent passes on this signal to all children.
After receiving SIGINT, the children save (using low-level file IO) their counter to {PID}.txt and end their work.
The parent awaits termination of all of its children and then exits.

## Graded stages

1. The parent creates children. The children print out their PID and indices and terminate. The parent awaits termination of all children and exits. **(3pt)**
2. The parent instructs the first child to do work by sending SIGUSR1 to it. The child performs the work in a loop. **(5pt)**
3. Handling of SIGUSR1 by the parent, handling of SIGUSR2 by the children. **(4pt)**
4. Handling of SIGINT. Saving the counters to the file by children. **(4pt)**
