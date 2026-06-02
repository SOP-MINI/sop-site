---
title: "UDP Test Task"
bookHidden: true
---

## L8: Cluster Computing

The dean of the MiNI Faculty has some good news: thanks to the high
course evaluation response rate, the faculty received a grant and was
able to buy new computers for the EDEN computing cluster. Faculty
members and PhD students are eager to use the new machines for advanced
computations, such as estimating the value of π. However, the new
hardware still needs a system for user access and job scheduling. Since
the entire grant has already been spent on the machines themselves, the
dean decided that the system will be implemented by students as part of
the Operating Systems course.

snippet with starting definitions `sop-eden-init.c`:
{{< includecode "sop-eden-init.c" >}}

[bash script for testing solutions]({{< resource sop-eden-test.sh >}})


## Stages:

1. Implement a UDP server that accepts messages in the following format:
   -   16 bytes containing the login (padded with zeros if needed) --- one
       of the logins listed in the `LOGINS` array,
   -   8 bytes containing the command (padded with zeros if needed) ---
       `RUN`, `EXIT`, `PAUSE`, `COMPUTE`, `LIST`, or `GATHER`,
   -   optional command parameters.
   
   For the `COMPUTE` command, the parameters consist of a non-empty
   sequence of pairs of 4-byte unsigned integers (`uint32_t`). The other
   commands do not take any additional parameters. All numbers are encoded
   in network byte order.
   
   A message may be at most `MSG_MAX` bytes long. The program should listen
   on the port given as its first command-line argument:
   
           ./sop-eden <port>
   
   If a malformed message is received, print a descriptive error message to
   the terminal, for example: `error: unknown user <login>`,
   `error: wrong message length <length>`, or
   `error: unknown command <command>`.
   
   If a valid message is received, print it together with its parameters to
   the terminal in the following format: `<user>: <command> <parameters>`.
   
   After receiving a valid `EXIT` command, the program should terminate
   cleanly.

2. The `COMPUTE` command requests new jobs for estimating the value of
   π, using the function
   `double compute_pi(const int count, const int* seed)` provided in the
   starter code. For each pair of numbers in the message, create a new job.
   The first number is the sample count and must not exceed 10 million.
   Make sure to check this limit; if it is exceeded, ignore that job and
   print an error message. The second number is the seed for the random
   number generator.
   
   Add each job to the job queue. The queue should support non-blocking
   insertion of any number of jobs in constant time, as well as
   constant-time removal (*hint:* a linked list satisfies these
   requirements).
   
   When the server receives the `LIST` command, it should send back all
   jobs currently in the queue that belong to the given user. If they do
   not fit into a single message (subject to the `MSG_MAX` limit), send
   them in multiple consecutive messages. Responses should be sent to the
   port number one greater than the sender's port --- for example, if the
   command was sent from port `8000`, the response should be sent to port
   `8001`.
   
   For each user, keep track of their current approximation of π.

3. Implement a thread pool consisting of `THREADS` worker threads for
   computing π. Each worker should take a job from the front of the
   queue (with proper synchronization) and process at most 1000 samples. If
   the job still has samples remaining afterward, put it back at the end of
   the queue with the remaining sample count updated accordingly.
   
   The newly computed result should then be combined with that user's
   current approximation using a properly weighted average.
   
   When the server receives the `GATHER` command, it should send back the
   user's current approximation of π as a text message formatted with
   12 digits after the decimal point.
  
4. When the server receives the `PAUSE` command, the thread pool should
   stop processing jobs belonging to that user until a `RUN` command is
   received.
   
   If a `PAUSE` command is received for a user who is already paused, or a
   `RUN` command is received for a user who is not paused, print the
   message `<username>: already paused/running` to the terminal and also
   send it back in the response.
   
   Note: if all users are paused, the program must not fall back to busy
   polling.

[Example solution of the first stage]({{< resource sop-eden.c >}})
