---
title: "L4 - Synchronization"
date: 2022-02-07T19:57:36+01:00
weight: 50
---

# Tutorial 4 - Synchronization

{{< hint info >}}
Introduction notes:

- Quick look at this material will not suffice, you should compile and run all the programs, check how they work, read additional materials like man pages. As you read the material please do all the exercises and questions. At the end you will find sample task similar to the one you will do during the labs, please do it at home.
- You will find additional information in yellow sections, questions and tasks in blue ones. Under the question you will find the answer, to see it you have to click. Please try to answer on you own before checking. 
- Full programs' codes are placed as attachments at the bottom of this page. On this page only vital parts of the code are displayed
- Codes, information and tasks are organized in logical sequence, in order to fully understand it you should follow this sequence. Sometimes former task makes context for the next one and it is harder to comprehend it without the study of previous parts.  
- Most of exercises require command line to practice, I usually assume that all the files are placed in the current working folder and that we do not need to add path parts to file names. 
- Quite often you will find $ sign placed before commands you should run in the shell, obviously you do not need to rewrite this sight to command line, I put it there to remind you that it is a command to execute.
- What you learn and practice in this tutorial will be required for the next ones. If you have a problem with this material after the graded lab you can still ask teachers for help.
{{< /hint >}}

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
{{< expand "Answer" >}} Mutex protects the access to the elements (variables,files) used in the variable condition so it remains unchanged when the code tests the condition. You must acquire the mutex prior to changing the state of those elements and prior to condition testing. {{< /expand >}}

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

## Dice game - barrier

Simulate a following dice game:
Each participant rolls a standard six-sided die simultaneously in 10 rounds. After each player rolled, one of the players concludes the round and assigns scores. The player with the highest roll in a given round is awarded one point. In the event of a tie for the highest roll, all tied players receive a point. The game concludes after 10 rounds, and the winner is determined by the player with the highest accumulated points.
Represent each player by a thread and use a barrier for the game synchronization.

<em>solution <b>prog23.c</b>:</em>
{{< includecode "prog23.c" >}}

How does the barrier works in this program?
{{< expand "Answer" >}} It is used to synchronize the threads at two key points within the thread function. The barrier ensures that all participating threads reach a specific point in their execution before allowing any of them to proceed. The barrier is initialized with a count of PLAYER_COUNT, which means it will block until PLAYER_COUNT threads have called pthread_barrier_wait. {{< /expand >}}

Which parts of the thread function are called concurrently?
{{< expand "Answer" >}} Each thread independently rolls a six-sided die, and the results are stored in the args->rolls array. This part of the code runs concurrently for all threads. {{< /expand >}}

How is the one player thread selected to conclude the round?
{{< expand "Answer" >}} The pthread_barrier_wait function returns PTHREAD_BARRIER_SERIAL_THREAD only for one thread (standard does not specify which one), and 0 for other threads. This mechanism ensures that the action is performed by a single thread in each round, preventing multiple threads from concurrently executing the same code that should only be executed once. {{< /expand >}}

## Source codes presented in this tutorial

{{% codeattachments %}}