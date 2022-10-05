---
title: "Test task on POSIX synchronization, network sockets, semaphores and threads"
date: 2022-02-01T19:36:27+01:00
---

## The task

Write a client-server application for matrix multiplication calculation. The client process is called with 2 parameters: port and address of the server process. Client process reads 8 floating point numbers from stdin (2 matrices 2×2) and sends the binary request to the process. Afterward, it reads the reply from server and displays calculated matrix (2×2).

Server process waits for incoming TCP connections. After receiving a connection, it creates a separate communication thread for given connection. The thread reads all 8 numbers from the client, creates the appropriate matrices and start 4 computing threads. Each thread is assigned for calculation a single output cell. After all threads had calculated the output value, the communication thread sends the results to the client. After 2 seconds, communication thread broadcasts to all calculation threads that they may terminate (via the synchronization mechanism). The calculation threads terminates (displaying the message “terminating thread”) and shortly after the communication thread terminates as well.

Server stops listening after receiving SIGINT (It handles all remaining connections). The server must not terminate if client process terminates before the response is sent.

## Stages

1. Server is waiting for incoming TCP connections. Creates separate thread to handle connections. The communication thread receives 8 floating point numbers and sends back to the client first 4 of them (5p)
2. Communication thread creates 4 processing threads. Processing threads are calculating the result matrix (each thread is calculating single output value). Threads “informs” the communication thread when all values are calculated. (5p)
3. The communication thread is sending the result to the client. Afterward, it “informs” its processing threads that they may terminate. (4p)
4. Server is properly handling the connection loss and SIGINT. (3p) 
