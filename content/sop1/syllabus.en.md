---
title: "Syllabus"
date: 2022-02-10T10:00:00+01:00
---

 - Lecture: 15h
 - Laboratory: 15h
 - Excercises: brak
 - Project: brak
 - ECTS: 2

## Course objectives

The first goal of the *Operating Systems I* course is to familiarize students with basics of operating system operation and design. The second goal is to develop student skills in correct application of system functions to design and implementation of simple multi-process/multi-thread applications which communicate using standard input/output channels and files.

## Expected course effects

At the end of the course students should:

  - have the knowledge of:
      - goals, properties and design of operating systems
      - fundamental models and methods of concurrent processing in computer systems
      - CPU scheduling - algorithms and quality indices
      - file systems: concept, organization, interface and effective use (in both synchronous and asynchronous mode)
  - be able to:
      - draw Gantts diagram and calculate quality indices for given CPU scheduling algorithms and process ready queue
      - design, code (in C language) and test simple multi-process/multi-thread applications (which communicate using standard input/output channels and files) properly taking into account operating system properties and interface (POSIX API)

## Correlation to other courses

Practical skills in C language programming and knowledge of "UNIX Systems Fundamentals" are obligatory.  
The "Operating Systems 2" (OPS2) is the continuation of this course.

## Lecture scope

**Introduction.** Tasks and properties of operating systems. Structures of computer systems and operating systems. System kernel structure and the interface.

**Processes and threads.** Concepts of process, lightweight process, thread and fiber; their life cycle and programmers interface. Implementation of concurrency of processes and threads. Co-routines and continuations. Signals and their handling in processes and threads. POSIX standard functions related to processes, signals and threads.

**Models and interfaces of file systems.** Files, file system layout. Operations on the file systems via programmers interface. Files access and protection, files access modes. POSIX standard functions related to manipulation of files and folders in both synchronous and asynchronous mode. C language standard library input/output streams manipulation functions.

## Laboratory scope

  - POSIX program execution environment
  - Processes and signals
  - Threads and mutexes

## Literature

1.  Silberschatz, P. Galvin, G. Gagne, *Applied Operating Systems Concepts*, J. Wiley & Sons.
2.  W. Stallings, *Operating Systems*, Prentice-Hall.
3.  A.S. Tanenbaum, H. Bos, *Modern Operating Systems*, Prentice-Hall.
4.  The GNU C Library Manual, <http://www.gnu.org/software/libc/manual/>
5.  POSIX.1-2017 documentation ([The Open Group Base Specifications Issue 7, 2018 edition IEEE Std 1003.1-2017](http://pubs.opengroup.org/onlinepubs/9699919799/))
