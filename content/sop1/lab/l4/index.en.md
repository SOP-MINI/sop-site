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

What is the semaphore used for?
{{< expand "Answer" >}} It keeps the count of threads that can still be started without exceeding  FS_NUM. Initially its value is set to 5 and before creating next thread it must be decreased by one. Zero value blocks the creation of the threads. Before the thread terminates it increases the semaphore by one. {{< /expand >}}

Why sem_trywait is used not just sem_wait before the new thread is created? What if we use sem_wait instead?
{{< expand "Answer" >}} We need to know that we reached the thread limit immediately so we can output deny message. With blocking wait we would wait until one of the threads finishes and increases the semaphore. {{< /expand >}}

What limits the concurrent alarms to 5?
{{< expand "Answer" >}} Semaphore, see the questions above. {{< /expand >}}

## Threads pool - conditional variables
Original author: **Jerzy Bartuszek**

Write a simple program that reads from "/dev/urandom" and writes its content 
to files. Every time user presses enter, the program reads random bytes and 
saves it in a file. The program is a multi-threaded application - each request 
is being handled in a separate thread. Each thread saves random bytes to its own file.
Program creates THREADS_NUM threads in advance and keeps idle threads in a
thread pool. After receiving SIGINT server stops accepting user input
and terminates its execution after handling all current requests.

<em>solution <b>prog22.c</b>:</em>
{{< includecode "prog22.c" >}}

It is worth of your time to analyze the above code in aspects not covered here (thread, mutex), please do it as an exercise.

Can a condition of conditional variable be based on regular variable value?
{{< expand "Answer" >}} Yes. {{< /expand >}}

Can a condition of conditional variable be based on a combination of regular variables' values?
{{< expand "Answer" >}} Yes. {{< /expand >}}

Can a condition of conditional variable be based on file content?
{{< expand "Answer" >}} Yes. {{< /expand >}}

Can a condition of conditional variable be based on file existence?
{{{< expand "Answer" >}} Yes. {{< /expand >}}

What are the limitations for the condition of conditional variable?
{{< expand "Answer" >}} Everything you can code that will return true or false, coder imagination defines the limit. {{< /expand >}}

Can we use conditional variable without any condition at all?
{{< expand "Answer" >}} Yes. it will become a pool of threads waiting for wakening as you need. {{< /expand >}}

Conditional variable must have a mutex, what is protected by it?
{{< expand "Answer" >}} Mutex protects the access to the elements (variables,files) used in the variable condition so it remains unchanged when the code tests the condition. You must acquire the mutext prior to changing the state of those elements and prior to condition testing. {{< /expand >}}

Can one mutex protect multiple conditional variables?<br>
{{< expand "Answer" >}} It can, but please consider the efficiency and parallelism of your code, it will be lowered. {{< /expand >}}

What are the parts of the condition for the conditional variable in the above code?
{{< expand "Answer" >}} The condition is solely based on the variable called "condition", all threads have access to this variable via pointers. {{< /expand >}}

How does the conditional variable works in this program?
{{< expand "Answer" >}} When main thread accepts a new connection it sets the "condition" variable to 1 and wakes one of waiting (waiting for the condition) threads. The thread that wakes, checks for "condition==1" and if it is true it handles the connection. {{< /expand >}}

Who should check for the condition to be true? The thread that wakes or maybe the one that is being wakened?
{{< expand "Answer" >}} The condition must be always checked by the thread being wakened. Even if the one that wakes checked it before it could have changed in meantime as the mutex was released and could have been acquired by some other thread to invalidate the condition! Generally it is better if the condition is checked also before signaling but sometimes it is not possible as wakening thread may not have access to all the condition components. {{< /expand >}}

What is cleanup handler in working thread used for?
{{< expand "Answer" >}} It is essential not to end the working thread without releasing the mutext that blocks the conditional (it would freeze entire program) . This handler releases the mutex in case of emergency exit. {{< /expand >}}
