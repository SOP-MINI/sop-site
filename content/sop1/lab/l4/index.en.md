---
title: "L4 - Synchronization"
date: 2022-02-07T19:57:36+01:00
weight: 50
---

# Tutorial 4 - Synchronization

## The alarm - Semaphores

Write a multi-threaded timer program. User inputs
number of seconds it needs to be counted to the program and awaits response. 
Program starts separate thread for each new request. The thread sleeps
for given time and outputs "Wake up" response. Then the thread exits.

Program has a limit of 5 concurrent threads. If it receives more
requests, it outputs immediate response "Only 5 alarms can be set at the time".

Limit on concurrent threads can be imposed with POSIX semaphore.

<em>solution <b>prog21.c</b>:</em>
{{< includecode "prog21.c" >}}

It is worth of your time to analyze the above code in aspects not covered here (thread, mutex), please do it as an exercise.

Why it is necessary to allocate memory before we start the thread, can this be avoided?
{{< expand "Answer" >}} One common structure is not enough as each thread has to have its own, you can have an array of structures (FS_NUM sized) but then you need to  manage this array in the way you manage memory so it is easier to allocate a structure on a heap. Naturally this memory has to be released somewhere, in this case, in the thread itself. {{< /expand >}}

<li>What is the semaphore used for?</br>
{{< expand "Answer" >}} It keeps the count of threads that can still be started without exceeding  FS_NUM. Initially its value is set to 5 and before creating next thread it must be decreased by one. Zero value blocks the creation of the threads. Before the thread terminates it increases the semaphore by one. {{< /expand >}}

Why sem_trywait is used not just sem_wait before the new thread is created? What if we use sem_wait instead?
{{< expand "Answer" >}} We need to know that we reached the thread limit immediately so we can output deny message. With blocking wait we would wait until one of the threads finishes and increases the semaphore. {{< /expand >}}

What limits the concurrent alarms to 5?
{{< expand "Answer" >}} Semaphore, see the questions above. {{< /expand >}}