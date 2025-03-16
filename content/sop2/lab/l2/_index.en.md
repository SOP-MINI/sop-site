---
title: "L2 - POSIX queues"
date: 2022-02-10T10:00:00+01:00
weight: 20
---

# Tutorial 6 - POSIX queues

The POSIX queues topic is significantly easier than the links topic, so this tutorial is shorter. Queues have message boundaries, can share access on read and write ends, can transfer large amounts of data in an atomic way, and can add priorities to the messages. All that makes them easier to use. They have one useful feature that requires some understanding - notifications.
We expect more self-studying from you. Please read lecture materials and man pages in addition to this page.

POSIX queue is the primary subject of this lab, but please expect the elements already practiced, especially threads, processes, and signals.

Please read all the materials listed below for the entry test and the task. You do not need to read all the content of the man pages listed. It is enough to know the purpose of the function, what parameters it takes (roughly, do not memorize all the parameters), and if it has flags, how they affect the outcome. Full man page invocations (execute it in the console) are listed. **Preparation references from the previous tutorial are still obligatory!**

- `man 7 mq_overview`
- `man 3p mq_open`
- `man 3p mq_close`
- `man 3p mq_unlink`
- `man 3p mq_getattr`
- `man 3p mq_setattr`
- `man 3p mq_send`
- `man 3p mq_receive`
- `man 3p mq_notify`
- `man 7  sigevent`

POSIX queues require linking with `librt`.
If you open an existing queue, you can expect some data in it. Do not assume it will be empty! If you need to make it empty, you can remove the queue before you create it again.

## Bingo!
Write a program that will simulate a simple variant of the BINGO game. It features processes (parent and children) and two message queues. Parent process creates n sub-processes (`0 < n < 100`, n is a parameter) and two message queues. The first one, `pout`, is used by the parents to send random numbers (range `[0-9]`) to children at 1-second intervals. The second one, `pin`, is for children to communicate their winnings and exits. Children compete over the numbers from `pout` -  one digit goes to only one child (not all of them). Sub-process reads the digit from the queue, compares it to its expected number (random number `E` chosen at the start), and if they match, sends a special "BINGO" message via the second queue `pin` along with the winning number. After "bingo", it terminates. Each child process must choose a random limit of numbers it is going to read from the queue and compare. Once the limit is reached without "bingo," it sends a special "GOODBYE" message via the second queue `pin` along with the terminating process number (its `n` number) and terminates. Asynchronously to sending numbers, the parent process must read the messages on the second queue (`pin`) and display proper information on the screen. Once all child processes are terminated, the parent process removes the queues and exits. **Single message size is limited to 1 byte only!**

### Solution
{{< includecode "mq_example.c" >}}

Please notice that sending a pointer along with the signal requires a slightly different signal handling function prototype (it has an additional siginfo_t* parameter), and during the installation, `SA_SIGINFO` flag was set. In this program, we do not send this pointer directly (notification does it for us); function `kill` does not allow for any extra data; to send a signal with a pointer (or an integer), you need to use the `big queue` function.
In code, uint8_t type (`stdint.h`) was used. It can hold unsigned numbers from the 0-255 range (one byte). Precise integer types (e.g., `int32_t`) are more portable between different architectures and compilers than traditional size unspecified types (like int). Type int can be stored on 2 or 4 bytes, long long type can be 4 bytes on 32 bytes Linux and 8 bytes on 64 bytes Linux. Type `int32_t` is always stored on 4 bytes. 
This program handles signals. Thus, proper restarting of uninterruptible functions is required. It is done with macro TEMP_FAILURE_RETRY added in almost all code despite the fact that only the parent process is handling the signals as it receives the notifications. Macros added in child slow down the code just a bit but also make the code more portable (also more reusable in students' works).
Notifications set on a queue are single-use only, i.e., after one notification, you must request another one if you wish to keep monitoring the queue. The notification is triggered only if the empty queue becomes nonempty. Only one process can be registered for the notification on the queue.

In the parent process, one uninterruptible function call is not restarted. Which one? Why is this restarting not required?
{{< answer >}} It is `mq_receive` in signal handling function. We do not expect an interruption as, by default, the signal being handled is blocked in the process for the duration of the signal handler. {{< /answer >}}

How does this code count the terminated child processes?
{{< answer >}} The code counts successful `waitpid` calls in `SIGCHLD` handler. You should not count the signals as they can marge. The counter of currently "alive" child processes is a global variable. {{< /answer >}}

When the notification signal is handled first, we request another notification, and then we read data from the queue. Why not the other way around?
{{< answer >}} If you do it in the reverse order, then after you read all the messages from the queue and just before you request another notification, something may be delivered to the queue. If that happens and you request the notification while the queue is not empty, you will not be notified until somehow the queue gets empty and, after that, not empty again. In this code, nothing is reading the queue without the notification; this program will stall, and no notification will ever be sent again! The sequence coded in this example is the right one. We set the notification on a nonempty queue; then we empty it. Now, if something arrives in the queue, we get the notification. {{< /answer >}}

Why is one of the queues in nonblocking mode?<br/>
{{< answer >}} The pin queues are in nonblocking mode due to the above explanation. If we block the reading in the signal handling function, it will block a whole process (the parent would stop sending the numbers on the pout queue). Blocking in asynchronous signal handler is a huge flaw of the code. {{< /answer >}}
How queue message priorities are used in this program, and how is it related to the 1-byte message limit?<br/>
{{< answer >}} In this code, priorities are not used to order the receive but as message types. We are forced to do it as we have only one byte for the information sent in the queue, and we need to know the number and what this number represents (bingo number or exiting process number). It is easier to use priorities than encode it all in one byte. {{< /answer >}}

As an exercise, modify this code to start a thread for notification.
You can simplify the program by creating a thread dedicated to reading all the messages from the queue; no notifications will be used then. Please do this as an exercise.
A huge part of the program's logic ended up in the signal handler; it is possible as this part does not have any common state (variables) with the rest of the program. What if we complicate it a bit and need some common variables? Can you change the program in such a way that all logic is in the main code, and the signal handler is limited only to a very simple global variable change? Do it as an exercise.

## Example tasks
Do the example tasks. During the laboratory you will have more time and the starting code. However, if you finish the following tasks in the recommended time, you will know you're well prepared for the laboratory.

- [Task 1]({{< ref "/sop2/lab/l2/example1" >}}) ~90 minutes
- [Task 2]({{< ref "/sop2/lab/l2/example2" >}}) ~120 minutes
- [Task 3]({{< ref "/sop2/lab/l2/example3" >}}) ~120 minutes
- [Task 4]({{< ref "/sop2/lab/l2/example4" >}}) ~150 minutes


## Source codes presented in this tutorial
{{% codeattachments %}}
