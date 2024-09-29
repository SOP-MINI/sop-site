---
title: "Syllabus"
---

 - Lecture: 30h
 - Laboratory: 15h
 - Excercise: none
 - Project: none
 - ECTS: 3

### Course objectives

The first goal of the second part of “Operating Systems” course is to familiarize students with advanced problems of contemporary operating systems design and implementation. The second goal is to develop student knowledge and skills necessary to design and implement (in POSIX/Unix/Linux environment) multi-process/multi-threaded applications with the use of various inter-process communication means, synchronization techniques and network communication.

### Expected course effects

At the end of the course students should:

  - have the knowledge of:
      - processor(s) time scheduling algorithms and their evaluation techniques
      - layout and effective utilization methods of system memory
      - principles of operation and effective utilization methods of virtual memory system
      - techniques of inter-process communication (shared memory, message queues, pipes)
      - basic means and patterns of synchronization and deadlock problem
      - network communication
      - computer systems security issues and resource protection techniques
  - can :
      - calculate quality indexes of CPU time scheduling algorithms and estimate average memory access time
      - discover deadlocks on resource allocation with the use of banker's algorithm
      - individually design, code (in C language) and test simple client-server application with the use of network sockets, message queues and pipes.

### Lecture scope:

- **Inter-process communication and synchronization.** Single system inter-process communication: named pipes (FIFO) and unnamed pipes (pipe), shared memory, message queues; synchronization of cooperating processes and threads: implementation and programmers interface. Deadlock problem: model, detection and solutions.
- **Implementation of file system.** Organization of selected file system types. Access and file protection, file access modes.
- **Input/Output subsystem.** Character and block devices; access modes. Input/Output devices drivers.
- **Memory management.** Organization of the main memory and processor memory access. Memory allocation algorithms. Process memory usage patterns. Virtual memory: concept and implementations (paging/segmentation on-demand); properties and programmers interface.
- **Network communication.** OSI networking model. Connection-oriented and connection-less sockets interface in networking programs (client/server model). Communication and synchronization problems in networking programs. Examples of network services.
- **Jobs scheduling.** Concurrency implementation of threads and processes. Job scheduling algorithms (for one and multiprocessor systems. Special requirements and implementation of real time systems job scheduling. Evaluation of scheduling algorithms.
- **Security and protection of computer systems.** Protection goals. Authorization, authentication, resources access control, system integrity, non-repudiation, confidentiality – concepts and applications. Security evaluation standards.
- **Review of contemporary operating systems.** Linux, MS Windows, QNX/Neutrino, FreeRTOS. Virtualization of computer systems.

### Laboratory scope:

  - Lab 1 FIFO/pipe (3h)
  - Lab 2 POSIX queues (3h)
  - Lab 3 Network sockets, pselect (3h)
  - Lab 4 Sockets, threads, shared memory, synchronization (3h)
  - Lab 5 Retake (3h)

### Literature:

 - basic:
   1.  Silberschatz, P. Galvin, G. Gagne, Applied Operating Systems Concepts, 8th ed., J. Wiley & Sons, 2010.
   2.  The GNU C Library Manual, <http://www.gnu.org/software/libc/manual/>
 - supplementary:
   1.  W. Stallings, Operating Systems, 4th ed., Prentice-Hall, 2001
   2.  S. Tanenbaum, H.Bos, Modern Operating Systems, 4th ed., Pearson, 2014
   3.  W.R. Stevens, Unix Network Progamming, [Vol. 1](http://www.kohala.com/start/unpv12e.html), Second Edition: Networking APIs: Sockets and XTI, Prentice Hall Inc.
   4.  W.R. Stevens, Unix Network Progamming, [Vol. 2](http://www.kohala.com/start/unpv22e/unpv22e.html), Second Edition: Interprocess Communications, Prentice Hall Inc.
   5.  M.J. Rochkind, Advanced Unix Programming (2nd edition), Addison-Wesley Professional, 2004
