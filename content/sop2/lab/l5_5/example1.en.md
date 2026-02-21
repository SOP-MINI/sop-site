---
title: "Task on POSIX message queues"
date: 2022-02-01T19:36:27+01:00
bookHidden: true
---

## Goal

Write 2 programs, client and server, that communicate via POSIX message queues.

The server program creates 3 message queues with names `PID_s`, `PID_d` and `PID_m` where `PID` is the server process ID. The server program prints the names of all queues to stdout.

The server waits for data in any of its queues, receiving a client PID and two integers. Then, computes the result: sum for `PID_s` queue, division for `PID_d`, and modulo for `PID_m`. The result is sent to the corresponding client queue (see below). The server should not terminate if the client queue is unavailable. After receiving `SIGINT` the server destroys its queues and terminates.

The client process is called with a single argument: the name of the server queue. The client creates its own queue (the name is the client's PID) and continues to read lines containing two integers until it encounters `EOF`. After receiving a line the client sends a message containing its PID and the two integers to the server queue and waits for a reply. The reply is displayed on stdout. If the reply does not arrive within 100 ms, the program terminates. While terminating, the program destroys its queue.

## Stages

1. The server creates its queues and displays the names of the queues. After 1 second, it destroys those queues and terminates. The client process creates its own queue, waits for 1 second, destroys its queue, and terminates.
2. Server reads the first message from `PID_s` queue. Sends the first answer back to the client. Ignores all errors. The client reads 2 integers from stdin and sends a single message to the server. It waits for the result and displays it.
3. The server handles all queues and calculates the proper results. Terminates at `SIGINT`. The client sends the messages until `EOF` is read or reply timeout occurs.
4. The queues should be removed on program termination. Full error handling.
