---
title: "Lec2. Processes"
date: 2022-02-05T22:35:27+01:00
draft: false
---

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
  - Login shell process creation.

## Reference

1.  Textbook (8th-10th ed.): chapter 3 (Process Concept, 3.1-3.4).
2.  Slides: [Processes.pdf]({{< resource "Processes_0.pdf" >}}), [POSIX\_processes.pdf]({{< resource "POSIX-processes_5.pdf" >}})
3.  Section 26 of the GNU C library documentation: [Processes](http://www.gnu.org/software/libc/manual/html_node/Processes.html#Processes)
4.  [Archive](../../../../common_src1/processes/S2.zip) of programs, that were used during lecture for experimentation. Note that the code is to expose interesting features of POSIX system and C library functions and not to give examples of good programming practices. The code is intended as a reminder of activities performed during a lecture. Perhaps it might stimulate further experimentation. Do not forget to share interesting results with the lecturer.
