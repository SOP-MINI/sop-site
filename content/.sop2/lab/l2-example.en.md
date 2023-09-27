---
title: "Task on POSIX message  queues"
date: 2022-02-01T19:36:27+01:00
bookHidden: true
---

## Goal

Write 2 programs: client and server that communicates via POSIX message queues. The server program creates 3 message queues with names ''PID_s'' ''PID_d'' and ''PID_m'', where PID is the number (PID identifier of the server). Server program prints the names off all queues to stdout.

Server reads from each queue the following data: the client PID and two floating point numbers from queue. Then computes the result: sum for _s queue, division for _d and modulo for _m. The result is send to the client queue. Server should not terminate if client queue is unavailable. Server terminates after receiving C-c and destroys its queues.

The client process is called with single argument: the name of server queue. Client creates its own queue (name is PID) and reads single line containing two digits from stdin. After encountering EOF program terminates. The Client sends the message containing the client PID and two digits to server queue and waits for a reply. The reply is displayed on stdout along with the name of server queue. If the reply does not arrive within 100 ms, the program terminates. While terminating, program is destroying its queue.

## Stages

1. Server creates its queues and displays names of the queues. After 1 second it destroys those queues and terminates. Client process creates its own queue, waits for 1 second and destroys its queue and terminates.(4p)
2. Server reads first message from _s queue. Sends the first digit back to the client. Ignores all errors. Client reads 2 digits from stdin and send singe message to the server. Then it reads and displays the result(4p)
3. Server handles all queues and calculates the proper results. Terminates at C-c. Client sends the messages until EOF is read or reply timeout occurs. (6p)
4. The queues should be removed on termination. Full error handling.(3p) 
