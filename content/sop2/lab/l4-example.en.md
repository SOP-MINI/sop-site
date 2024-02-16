---
title: "Zadanie testowe z sieci"
date: 2022-02-01T19:36:27+01:00
bookHidden: true
---

## Goal

Write client server TCP application. Client prints its PID on stdout, connects to the server and sends its PID at a text. Server sends back the sum of digits in the PID as int16_t. Client prints the result and exits. Server accepts clients until C-c. On SIGINT it exits and prints the highest sum it calculated. Example:

## Example

```
./server 2000&
./client localhost 2000
PID=1244
SUM=11
./client localhost 2000
PID=1245
SUM=12
killall -s SIGINT server
HIGH SUM=12
```

## Stages

1. Server accepts only one connection, reads text data from the client and prints it on the stdout. Client connects and sends text data
2. Server accepts many connections, counts the sum of digits and sends the numer (int16_t) back to the client, client prints the result
3. Server properly handles C-c, prints maximum sum.
4. All types of broken connection on the socket are properly checked and handled. 
