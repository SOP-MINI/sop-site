---
title: "Task on pipe/FIFO"
date: 2022-02-01T19:32:59+01:00
bookHidden: true
---

## Goal

Write 2-process application that exchanges text messages via pipes. Communication must be bidirectional.

Parent process starts the communication by passing the sole program argument to the child process. From now on, both processes follow the same steps: remove random letter from the string, print it with process PID and pass it back to the other process. The process that receives empty string “” shall terminate. The other process should detect broken link condition and exit as well.

Example:

```
 ./prog abcd
PID 1011:abd
PID 1012:bd
PID 1011:d
PID 1012:
```

## Stages

1. Create processes and pipes, close unused descriptors. After each pipe and close function call print out (PID, function, descriptor number). (6p)
2. Both processes send random substring of program argument (argv[1]) over the pipe, read from its pipe, print the incoming data and exit. At this stage you must be able to handle variable length messages. (4p)
3. Parent process sends argument to the child, child sends it back unchanged and so on until C-c is pressed. At each cycle process should print the message and its PID (2p)
4. Add letter reduction, STOP condition and detect broken link to exit (5p) 
