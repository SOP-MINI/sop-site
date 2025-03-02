---
title: "L1 - FIFO/pipe"
date: 2022-02-10T10:00:00+01:00
weight: 10
---

# Tutorial 1 - FIFO/pipe

{{< hint info >}}
Introduction notes:
- All materials from OPS1 are still obligatory!
- Quick look at this material will not suffice, you should compile and run all the programs, check how they work, read additional materials like man pages. As you read the material please do all the exercises and questions. At the end you will find sample task similar to the one you will do during the labs, please do it at home.
- Codes, information and tasks are organized in logical sequence, in order to fully understand it you should follow this sequence. Sometimes former task makes context for the next one and it is harder to comprehend it without the study of previous parts.  
- Most of the exercises require command line to practice, I usually assume that all the files are placed in the current working folder and that we do not need to add path parts to file names. 
- Quite often you will find $ sign placed before commands you should run in the shell, obviously you do not need to rewrite this sight to command line, I put it there to remind you that it is a command to execute.
- What you learn and practice in this tutorial will be required for the next ones. If you have a problem with this material after the graded lab you can still ask teachers for help. 
- In this tutorial, all synchronization problems are solved only by use of FIFO/pipe. Adding another IPC like semaphore would cancel some of the restrictions (like multi process read from links). The purpose of this limitation is to highlight what you can do only on links and to show that mostly you do not need extra IPC to work on FIFO's and pipes.
- Do remember that links are unidirectional communication means. In case of FIFO it is easy to remember because you decide on the direction at open time. In case of pipe you get a pair of descriptors and you have to remember that index[0] is for reading and index[1] is for writing. Please notice the symmetry with standard input and output descriptors numbers (0 for input - reading, 1 for output - writing). I use the therm "other end" in the relation the read end and write end. Read end is the "other end" for write end. 
- If one link is used by multiple writing processes and one reading process then single write to it is  guaranteed to be continuous provide the write size does not exceed PIPE_BUF constant. If you plan such a communication model based solely on link you can not exceed this size in one message.
- There is no analogy for reversed model (to the one above) i.e. when multiple processes are reading from the pipe. In theory such a reads can interfere with each other.
- Braking of the link (caused by closing the link by all the processes on the opposite end or by killing those processes) will result in EOF (End of File, zero bytes read) state on reading end  or by SIGPIPE signal delivery to the writing end. The default reaction on the SIGPIPE signal is to kill the process. If this signal is blocked, ignored or handled then write to the link will result in EPIPE error.
- It is worth knowing that the same breaking rules apply to the sockets.
- Pipe is created already in connected/open state in opposite to the FIFO that must be connected before you can use it. If no O_NONBLOCK is applied to the link descriptor then the connecting process or thread is blocked until the connection is established on the other end of the link. If you use O_NONBLOCK then open process finish immediately but the link is unusable until it gets connected on the other end. If you try to write to unconnected link you will get ENXIO error and if you try to read from it EOF will be reported.
- PIPE_BUF on  Linux equals to 4kb
- According to  POSIX, PIPE_BUF has to be at least 512 bytes.
{{< /hint >}}

## Questions

Do you need to divide the communication into PIPE_BUF parts in case of one to one connection i.e.  one process writes and one reads?
{{< answer >}} There is no need to do it, there is no competition over the link, no interference from other processeses.  {{< /answer >}}
Can you open a pipe or FIFO to communicate inside of one process/thread?
{{< answer >}} It is possible and allowed but very rare case though.  {{< /answer >}}
What extra precaution has to be taken in to consideration in above scenario?
{{< answer >}} Deadlock, the link has internal buffer of PIPE_BUF size, if you overflow it the process/thraad will block as it cannot at the same read the excess of data from the link. This blockade can be prevented with O_NONBLOCK mode. {{< /answer >}}
Is quick sending a few portions of data (with multiple writes) of total size below PIPE_BUF guaranteed to be continous?
{{< answer >}} NO, the guarantee is only valid for ONE write. {{< /answer >}}
In case of multiple writing processes that need to send messages of variant length, is sending them in atomic sizes enough for the receiving process to properly receive them?
{{< answer >}} NO, the reading end must know how many bytes belong to each message. If size are mixed then it is impossible  to guess. You can add the size of the message at it beginning and the reader can first read the size, then the rest of the message or you can always use fixed size of the messages by adding something neutral (zero code may be a good choice for textual data) do fill the missing end part of the message. {{< /answer >}}
When can a write of  2*PIPE_BUF bytes to the link be interrupted?  How do you know about the interruption? The question concerns all types of possible interruptions - caused by signal handling function, CPU scheduler or broken link.
{{< answer >}} Before anything is sent it (by signal), it is reported as EINTR error. After sending PIPE_BUF bytes (scheduler), then write returns less than expected size of 2*PIPE_BUF. {{< /answer >}}
Assuming very intensive signal handling in the program, how many times can the above write be interrupted?
{{< answer >}} You can not tell exactly, multiple times before it starts and multiple times after first PIPE_BUF bytes transfer. {{< /answer >}}
When can be interrupted the transfer of PIPE_BUF bytes?
{{< answer >}} It can be interrupted if you handle signals in the program only before any data gets transmitted (EINTR error) or by broken pipe. {{< /answer >}}
What does it mean if you read zero bytes from the link?
{{< answer >}} EOF - broken pipe/fifo. {{< /answer >}}
What did you forget to check if your pipe/fifo program terminates abruptly with message about signal interruption?
{{< answer >}} You forgot to check for broken pipe condition on write end, you should ignore SIGPIPE and check for EPIPE errors on write statements. {{< /answer >}}
Can EPIPE be reported on link read?
{{< answer >}} NO, SIGPIPE/EPIPE concerns only writing to the link! {{< /answer >}}
Can writing to pipe report zero as return value?
{{< answer >}} Assuming you did not try to write zero bytes it can not. EOF concerns only reads! {{< /answer >}}


## Task 1 - FIFO

Goal:

Write client server application, communication between clients and the server is based on single shared FIFO.
The server reads data from the connection,removes all non-alphanumerical characters from this input and prints processed
data on the standard output along with senders PID number.
Client input data comes from the file opened by the client (name of the file is one of the client parameters).

Client application terminates as soon as all the file is sent. Server application terminates as soon as all clients
disconnect from its FIFO. Both programs should correctly react to the lost connection on FIFO!

Let's split the solution into the stages.

What you should know:

- `man 7 fifo`
- `man 7 pipe`
- `man 3p mkfifo`
- `man 3 isalpha `
- `man 0p limits.h ` 

Makefile common for all the codes in this tutorial:

```makefile
CC=gcc
CFLAGS= -std=gnu99 -Wall
```

### Stage 1 

- Prepare simplified server that will create FIFO and will read data from it. What comes out of the FIFO will be filtered (alphanumeric chars only) and printed on the screen.
- Use command `cat` as the client. 

Solution **prog21a_s.c**:
{{< includecode "prog21a_s.c" >}}

Execution: `./prog21a_s a & sleep 1; cat prog21a_s.c > a`

{{< hint warning >}}
Please remember to check for system function (open,close,read etc.) errors or even better all errors. By this you can tell a good programmer from the bad one.
{{< /hint >}}

- Why there is one second sleep in execution command between a server start and a cat command?
{{< answer >}} It allows enough time for the server to start up and create the fifo file "a", without it, the cat command can be first to create "a" as a regular file, then the server would not open a fifo but a regular file instead. You would not notice the problem if "a" already exists and is a FIFO, to reproduce the problem make sure "a" is missing and  remove the sleep 1 from the execution commands. {{< /answer >}}
- What is the type of "a" in file system and how to check it?
{{< answer >}} $ls -l  - it is a fifo "p" {{< /answer >}}
- Why EEXIST reported by mkfifo is not treated as a critical error?
{{< answer >}} We do not remove the fifo file in this stage, if you run the program for the second time "a" will be already in the filesystem and it can be safely reused instead of forcing the user to manually remove it every time. {{< /answer >}}
- Isn't reading from fifo char by char inefficient?
{{< answer >}} The data is read from the fifo buffer, the only extra  overhead includes the one extra function call. It is not the fastest way but it also can not be called very inefficient. {{< /answer >}}
- Isn't writing char by char inefficient?
{{< answer >}} In this program we write to buffered stream, the extra overhead is minimal, but when you write chat by char to unbuffered descriptor then the overhead becomes a serious problem. {{< /answer >}}
- How can you tell that the link does not have and will not have any more data for the reader?
{{< answer >}} EOF - broken pipe detected on read occurs when all writing processes/threads disconnect the link and the buffer is depleted.  {{< /answer >}}

### Stage 2

- Prepare complete client program, it reads files and sends it via FIFO in PIPE_BUF chunks.
- All chunks sent must be of PIPE_BUF size, including the last one.
- Each chunk sent must be tagged with PID nuber

Solution: **prog21_c.c**:

{{< includecode "prog21_c.c" >}}

Execution: `./prog21a_s a & ./prog21_c a prog21a_s.c`

- Please notice how PID is stored in binary format in the buffer. This binary method saves time on conversions from and to text and the result always has the same easy to calculate size (sizeof(pid_t)). Technically it takes only some type casting and cleaver buffer shift to preserve the PID data at the beginning. You can use structures to store more complex variant size data  in the same way and provide both communicating programs were compiled in the same way (structures packing problem) it will work.
- This time the program opens the fifo for writing instead of reading like server, please remember that fifos are unidirectional.

- Why this time sleep was not required in execution commands?
{{< answer >}} Now it does not matter who creates the fifo and what is the sequence of it's opening , the other side will always wait. The client program can create the fifo with mkfifo (unlike cat command) and  the nonblocking mode is not used. {{< /answer >}}
 - Why constant size of send messages is important in this program?
{{< answer >}} This is the simplest way for the server to know how many bytes comes from one client. {{< /answer >}}
- What is memset used for?
{{< answer >}} For a quick way to fill the missing part of the last buffer (to the required size of PIPE_BUF) with zeros. Zeros at the end of the string are natural terminators. {{< /answer >}}
- Can you send the zeros filling after the last part of the file in separated writes?
{{< answer >}} NO, it can mix with the data from the other clients. {{< /answer >}} 
- How this program will react to broken pipe (fifo in this case but we name any disconnected link in this way) ?
{{< answer >}} It will be killed by SIGPIPE. {{< /answer >}}


### Stage 3

- Add chunking on the server side.
- Add FIFO removal

Solution **prog21b_s.c**:

{{< includecode "prog21b_s.c" >}}

Execution: `./prog21_c a Makefile & ./prog21_c a prog21b_s.c & ./prog21_c a prog21_c.c & sleep 1; ./prog21b_s a`

- Please notice the unlink call at the end of the program, it removes fifos as well as regular files.
- Please notice how PID is decoded from the buffer - nearly the same way it was coded.

- Why sleep is used once more in the execution commands?
{{< answer >}} Without this sleep it may happen that if one of the clients and the server are given enough CPU time earlier than the rest of the clients, they will be able to start, connect overt the FIFO and exchange the data. When sole connected client exits the server will assume that it should terminate before other client even got chance to connect to FIFO! The abandoned clients will hang for long on the fifo without a server to serve them. {{< /answer >}}
- How server knows how much bytes comes from one client?
{{< answer >}} Messages have fixed size of PIPE_BUF. {{< /answer >}}
- Can we send blocks of more bytes than PIPE_BUF at a time?
{{< answer >}} No, it is not guaranteed to be continuous/atomic. {{< /answer >}}

## Task 2 - pipe

Goal:

Write n-process program where n child processes communicate with parent over one shared pipe R and parent communicates with children using n dedicated pipes P1,P2,...,Pn
On C-c parent process chooses random pipe Pk (where k in [1,n]) and sends random [a-z] char to it. On the same signal child processes terminate with 20% probability. Child process that can read  char C on its dedicated pipe should send a random size [1,200] buffer of chars C via parent pipe R.
Parent process should print everything it receives on R pipe as soon as possible and terminate when all the child processes are gone.
Let's split the solution into 2 stages.

- Please notice that broken pipe in parent does not need to terminate the program, it must stop using this one pipe instead.
- 200 bytes is the maximum buffer size, will it be atomic? On Linux and all POSIX compatible platforms yes but the problem is more complex, what if you need a bit  larger buffers? You can derive your size from PIPE_BUF (e.g. PIPE_BUFF/4) or test your max size against the PIPE_BUF and if it exceeds make it equal to PIPE_BUF. 

What you should know:
- `man 3p pipe`

### Stage 1

- Create n child processes.
- Create pipes.
- Close unused descriptors.
- Initiate random numbers.
- Parent process awaits data on pipe R and prints them on the stdout, it terminates after all data is processed.
- Child process sends random char on R pipe and exits.


Solution **prog22a.c**:
{{< includecode "prog22a.c" >}}


- It is important to close all unused descriptors, this pipe program has a lot to close, please make sure you know which descriptors are to be closed right at the beginning.
- Likewise descriptors unused memory should be released, make sure you know what heap parts should be deallocated in "child" process.
- [a-z] characters randomization should be obvious. If it is not try to build more general formula from this example on paper, try to apply it to other ranges of chars and numbers.
- Sometimes (set n=10 for most frequent observation) program stops with the message : "Interrupted system call" , why? 
{{< answer >}}  SIGCHLD handling interrupts read before it can read anything. {{< /answer >}} 
- How can we protect the code from this interruption? 
{{< answer >}} The simplest solution would be to add macro: TEMP_FAILURE_RETRY(read(...)). {{< /answer >}}
- How the program reacts on broken pipe R?
{{< answer >}} This is natural end of the main loop and the program because it happens when all the children disconnect from R, they do it when they are terminating. {{< /answer >}}
- Why the parent process do not wait for children at the end of the process code? 
{{< answer >}}  In this code all the children must terminate to end the main parent loop, when the parent reaches the end of code there are no children to wait for as they all must have been waited for by SIGCHILD handler. {{< /answer >}}


# Stage 2
- Add SIGINT handling.
- Protect system function from interruption by the signal handling function (all the code).
- Add missing code.

Solution **prog22b.c**:
{{< includecode "prog22b.c" >}}

- Please notice that messages sent via pipe R have variant size - first byte states the message size.
- As the size is coded in one byte it cannot exceed 255, the constant MAX_BUFF can not be increased beyond that value. It is not obvious and appropriate comment was added to warn whoever may try to do it. This is an example of absolutely necessary comment in the code.
- This task algorithm invalidates the child pipe descriptors as children "die" with 20% probability on each SIGINT. To not try to send the letter to such a "dead" descriptor it must be somehow marked as unused. In this example we use value zero to indicate the inactivity. Zero is a perfectly valid value for descriptor but as it is  used for standard input and normally we do not close standard input zero can be used here as we do not expect pipe descriptor to get this value. Close function does not change the descriptor value to zero or -1, we must do it the code.
- Random selection of child descriptor must deal with inactive (zero) values. Instead of reselection the program traverses the array in search for the nearby active  descriptor. To make the search easier the buffer is wrapped around with modulo operand, to prevent infinite search in case of empty array extra counter cannot exceed the longest distance between the hit and the last element in the array.

---

- Does this program correctly closes all its pipes? 
{{< answer >}} No because in signal handler we used `exit()` we never reach closing. The program must be fixed. For example in signal handler we can set variable to signal that process should exit and then in `child_work` we can check it instead of using `TEMP_FAILURE_RETRY` {{< /answer >}}
- Where the parent program waits for the SIGINT signal? There is no blocking, no sigsuspend or sigwait?
{{< answer >}} Most of the time program waits for input on the first read in parent's main loop, if it is interrupted by signal it exits with EINTR error. {{< /answer >}}
- Please notice that nearly every interruptible function in the code is restarted with TEMP_FAILURE_RETRY macro, all but one above mentioned read, why?
{{< answer >}} With macro restarting this read, it would be impossible to react on the delivery of the signal and the program wouldn't work. In this case the interruption is a valid information for us!  {{< /answer >}}
- Can we use SA_RESTART flag on signal handler to do the automatic restarts and get rid of TEMP_FAILURE_RETRY?
{{< answer >}} No,  for the same reasons as described in the previous answer. Additionally our code would get less portable as I mentioned in preparation materials for L2 of OPS1. {{< /answer >}}
- Why not all C-c keystrokes result in printout from the program?
{{< answer >}}  It may be due to the chosen child terminating at the same SIGINT (it has 20% chance to do it), signals may merge in the time the main parent loop is still processing the previous C-c and is not waiting on the before mentioned read. {{< /answer >}} 
- The second reason can be eliminated if you change global variable last_signal to act as the counter that gets increased every time signal arrives, then you can send as many chars to random children as your counter tells you. Please modify the program in this way as an exercise.
- Can child process marge the signals?
{{< answer >}} Only in theory as the signals are immediately and quickly handled. {{< /answer >}}
- Why parent can read the data from the pipe R in parts (first size, then the rest)  and child must send data to R in one write? 
{{< answer >}} To prevent mixing of the data from various children, see the remarks at the beginning of this tutorial. {{< /answer >}}
- Why the program ignores SIGPIPE, is it safe?
{{< answer >}} It is not only safe but also necessary, without it the attempt to send the char to "dead" child would kill the whole program. In many cases the fatal termination is NOT the CORRECT WAY of DEALING with BROKEN PIPE! {{< /answer >}}
- What is the STOP condition for a main parent loop?
{{< answer >}} When parent reads zero bytes from R, it means broken pipe and it can only happen when all children are terminated. {{< /answer >}}
- A proper reaction to broken pipe is always important, check if you can indicate all broken pipe checks in the code, both on read and write. How many spots in the code can you find?
{{< answer >}}  4  {{< /answer >}}
- Why we use unsigned char type, what would happen if you remove unsigned from it?
{{< answer >}} For buffers larger than 126, buffer sizes read from R would we treated as negative! {{< /answer >}}
- Why SIGINT is first ignored in the parent and the proper signal handler function is added after the children are created?
{{< answer >}}  To prevent the premature end of our program due to quick C-c before it is ready to handle it. {{< /answer >}}
- Is SIGCHLD handler absolutely necessary in this code?
{{< answer >}} It won't break the logic, but without it zombi will linger and that is something a good programmer would not accept. {{< /answer >}}

## Source codes presented in this tutorial

{{% codeattachments %}}


## Example tasks
Do the example tasks. During the laboratory you will have more time and the starting code. However, if you finish the following tasks in the recommended time, you will know you're well prepared for the laboratory.

- [Task 1]({{< ref "/sop2/lab/l1/example1" >}}) ~60 minutes
- [Task 2]({{< ref "/sop2/lab/l1/example2" >}}) ~90 minutes
- [Task 3]({{< ref "/sop2/lab/l1/example3" >}}) ~120 minutes
- [Task 4]({{< ref "/sop2/lab/l1/example4" >}}) ~120 minutes
- [Task 5]({{< ref "/sop2/lab/l1/example5" >}}) ~120 minutes

## Additional materials

- <http://cs341.cs.illinois.edu/coursebook/Ipc#pipes>
- <http://cs341.cs.illinois.edu/coursebook/Ipc#named-pipes>
