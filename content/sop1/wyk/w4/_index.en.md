---
title: "Processes"
date: 2022-02-05T22:35:27+01:00
weight: 40
---

# Processes

## Scope

- Process concept. Process models: UNIX (POSIX), MSWin (Win32).
- Diagram of process state.
- Process Control Block. Process context. Switching CPU between processes.
- Scheduling Queues. Schedulers: short-term, long-term, medium-term.
- Process creation: Models: POSIX, Win32.
- Process termination. POSIX: role of wait(), orphan, zombie.
- Run-time POSIX process environment: environment variables, 3 initial streams (stdin, stdout, stderr). The startup module (`crt0`)
- POSIX process life-cycle
- Inheritance across `fork` and `exec` system calls
- C library function packaging of the `exec` system call.
- Process ids: PID, GID, real/effective ids. setuid/setgid.
- Process groups

## Reference

1. Textbook (8th-10th ed.): chapter 3 (Process Concept, 3.1-3.4).
2. [Lecture presentation](https://link.excalidraw.com/p/readonly/EdnsxZsbiJBVHZv7NMS1) ([PDF]({{< resource OPS1_Processes.pdf >}}))
3. [Lecture code samples](https://gitlab.com/SaQQ/sop1/-/tree/master/02_processes?ref_type=heads)
4. Section 26 of the GNU C library documentation: [Processes](http://www.gnu.org/software/libc/manual/html_node/Processes.html#Processes)
5. Old Slides: [Processes.pdf]({{< resource "Processes_0.pdf" >}}), [POSIX\_processes.pdf]({{< resource "POSIX-processes_5.pdf" >}})
