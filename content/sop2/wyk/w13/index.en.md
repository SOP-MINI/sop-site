---
title: "Lec13. Real-time systems"
date: 2022-02-05T16:17:06+01:00
---

## Scope of the lecture

  - Real-time (RT) systems
      - hard RT vs soft RT: characteristics and applications
      - RT systems vs desktop systems: display/user interaction, memory management, virtual memory, single/multi-purpose
      - "must have" of RT: preemptive, priority-based scheduling, preemptive kernels, minimized interrupt and dispatch latency
      - RT-specific CPU scheduling:
          - Rate-Monotonic Scheduling (RMS)- optimal assignment of static priorities for periodic tasks, pretty low worst-case CPU utilization (N(2^1/N-1))
          - Earliest Deadline First (EDF) scheduling - does not require that processes be periodic, nor must a process require a constant CPU time per burst; requires that a process announces its deadline to the scheduler when it becomes runnable.
          - Proportional share scheduling
      - POSIX scheduling policies
          - Real-time (RT): SCHED\_FIFO, SCHED\_RR, SCHED\_SPORADIC
          - Non RT: SCHED\_OTHER
      - Example RT systems - VxWorks, RTLinux

## Reference

1.  Slides: [RT\_en.pdf]({{< resource "RT_en_4.pdf" >}})
2.  Textbook: chapter 19 - "Real-Time Systems".
3.  `man: sched(7)`
