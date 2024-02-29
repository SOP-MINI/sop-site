---
title: "Task on POSIX message queues"
date: 2022-02-01T19:36:27+01:00
bookHidden: true
---

## Goal

Write 2 programs, client and server, that communicate via POSIX message queues. The server program creates 3 message queues with names `PID_s`, `PID_d` and `PID_m` where PID is the number (PID identifier of the server). The server program prints the names off all queues to stdout.

The server reads the following data from each queue: the client PID and two floating point numbers from the queue. Then, computes the result: sum for `PID_s` queue, division for `PID_d`, and modulo for `PID_m`. The result is sent to the client queue. The server should not terminate if the client queue is unavailable. The server terminates after receiving C-c and destroys its queues.

The client process is called with a single argument: the name of the server queue. The client creates its own queue (the name is PID) and reads a single line containing two digits from stdin. After encountering `EOF` the program terminates. The client sends the message containing the client PID and two digits to the server queue and waits for a reply. The reply is displayed on stdout with the name of the server queue. If the reply does not arrive within 100 ms, the program terminates. While terminating, the program is destroying its queue.

## Stages

1. The server creates its queues and displays the names of the queues. After 1 second, it destroys those queues and terminates. The client process creates its own queue, waits for 1 second, destroys its queue, and terminates.
2. Server reads the first message from `PID_s` queue. Sends the first digit back to the client. Ignores all errors. The client reads 2 digits from stdin and sends a single message to the server. Then, it reads and displays the result.
3. The server handles all queues and calculates the proper results. Terminates at C-c. The client sends the messages until `EOF` is read or reply timeout occurs.
4. The queues should be removed on termination. Full error handling.
