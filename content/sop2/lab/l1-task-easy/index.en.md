---
title: "Task on pipe/FIFO"
date: 2022-02-01T19:32:59+01:00
bookHidden: true
---

## Goal

Write a program that uses pipes to communication between three processes. The first process (parent) is connected using pipe to second process, which is connected to third process. The third process is connected back to the parent process, so all processes exchange messages in ring topology.

After creating processes and pipes parent process sends `1` (as a null-terminated string). After that each process after receiving new number prints it to terminal with its `PID`. Then it add to that number random value in range `[-10,10]` and sends to the next process. If one of process receives `0` it should terminate. Other process should detect broken pipe and also terminates. All resources (memory, pipes) should be correctly released.

## Stages

1. Create processes and pipes, correctly close unused descriptors. Each process should print descriptor it uses and closes. Parent process wait for children and then terminates.
2. Each process sends random number in range `[0-99]` and prints number received whit its `PID` to stdout. At this stage you should handle variable length messages.
3. Parent process sends `1`, which travels without any change until receiving C-c.
4. Add number changing, STOP condition and pipe break check.
