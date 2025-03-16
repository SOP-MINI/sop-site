---
title: "Task on POSIX message queues"
date: 2022-02-01T19:36:27+01:00
bookHidden: true
---

## Employees

Create a task distribution system for employee processes using POSIX message queues.  
The system will simulate work by adding two random floating-point numbers (range 0-100.0) and putting the process to sleep for a random duration (500 ms - 2000 ms).  

The main process (server) adds new tasks to the queue at random intervals (`T₁` - `T₂` ms, where `100 <= T₁ < T₂ <= 5000`).  
Each task consists of a pair of random floating-point numbers from the range `[0.0, 100.0]`.  

At the start, child processes (N workers, where `2 <= N <= 20`) are created and register themselves in the task queue named `"task_queue_{server_pid}"`.  
Workers, while waiting for tasks, retrieve available tasks from the queue when they are idle.  
Each worker returns the results through its own queue named `"result_queue_{server_pid}_{worker_id}"`.  

**Note:** The program should support multiple instances without queue name collisions!  

## Steps:

1. The main process creates N workers, who after performing a random sleep (500 ms - 2000 ms) finish their work.  
   At the start, the server prints `"Server is starting..."` and after all workers have finished, it prints `"All child processes have finished."`  
   Workers print `"[{worker_pid}] Worker ready!"` at startup and `"[{worker_pid}] Exits!"` upon finishing.  

2. The server creates `5 * N` tasks at random time intervals (`T₁` to `T₂` ms) and adds them to the queue.  
   The server informs when a task is added: `"New task queued: [{v1}, {v2}]"` or if the queue is full: `"Queue is full!"`.   
   Workers, upon receiving a task, print: `"[{worker_pid}] Received task [{v1}, {v2}]"`, sleep randomly (500-2000ms),  
   print the result as `"[{worker_pid}] Result [{result_value}]"`, and wait for the next task. They stop working after completing 5 tasks.  

3. Workers send results to the server through their individual queues and inform about it on standard output:  
   `"[{worker_pid}] Result sent [{value}]"`  
   The server receives results and prints: `"Result from worker {worker_pid}: {value}"`  

4. The main process continues to create tasks until it receives a `SIGINT` signal, after which it informs workers to stop (via the queue).  
   The server waits for workers to finish their current tasks before terminating.  
   Workers finish after receiving the stop notification from the server (they complete only the tasks they started—tasks still in the queue are ignored).  
   All resources are properly released.  

## Example Program Execution:

```sh
./sop-qwp 3 500 2000
Server is starting...
[5831] Worker ready!
[5832] Worker ready!
New task queued: [84.65, 17.80]
[5831] Received task [84.65, 17.80]
[5833] Worker ready!
[5831] Result sent [102.46]
Result from worker 0: 102.46
New task queued: [25.87, 10.67]
[5832] Received task [25.87, 10.67]
New task queued: [84.04, 7.21]
[5833] Received task [84.04, 7.21]
[5832] Result sent [36.55]
Result from worker 1: 36.55
[5833] Result sent [91.25]
Result from worker 2: 91.25
New task queued: [83.46, 35.65]
[5831] Received task [83.46, 35.65]
New task queued: [38.44, 27.78]
[5832] Received task [38.44, 27.78]
[5831] Result sent [119.12]
Result from worker 0: 119.12
^CClosing server...
[5833] Exits
[5831] Exits
[5832] Result sent [66.23]
[5832] Exits
Result from worker 1: 66.23
All child processes have finished.
```
