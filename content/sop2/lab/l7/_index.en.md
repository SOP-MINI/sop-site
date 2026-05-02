---
title: "L7 - sockets and epoll"
weight: 40
---

# Tutorial 7 - Sockets and epoll

{{< hint warning >}}
In this tutorial we use `epoll` functions family to wait on multiple descriptors. `epoll` is not a part of POSIX, but Linux extension. If you want to write portable code, you should look at `select` or `poll` functions - which are standarized, but have worse performance and are less convenient to use.
{{< /hint >}}

Introduction notes:

1. It's worth to familiarize yourself with [netcat]({{< ref "/sop2/lab/netcat" >}}) befaore the lab
1. All materials from OPS1. OPS2 are still obligatory, especially tutorials on threads and processes!
1. Quick look at this material will not suffice, you should compile and run all the programs, check how they work, read additional materials like man pages. As you read the material please do all the exercises and questions. At the end you will find sample task similar to the one you will do during the labs, please do it at home.
1. Codes, information and tasks are organized in logical sequence, in order to fully understand it you should follow this sequence. Sometimes former task makes context for the next one and it is harder to comprehend it without the study of previous parts.  
1. Most of exercises require command line to practice, I usually assume that all the files are placed in the current working folder and that we do not need to add path parts to file names. 
1. Quite often you will find $ sign placed before commands you should run in the shell, obviously you do not need to rewrite this sight to command line, I put it there to remind you that it is a command to execute.
1. What you learn and practice in this tutorial will be required for the next ones. If you have a problem with this material after the graded lab you can still ask teachers for help. 
1. In many cases in the below codes  post-interruption restarting  macro TEMP_FAILURE_RETRY was deployed even though not all the programs handle the signals. It was done due to the fact that most of the presented code will be reused by students in their solutions that may require signal handling and the fact that a lot of presented functions can be reused to build a library for your future codes, thus it should be as portable as possible.
1. It is essential to consider the architectural differences when planning the protocol that is used in communication over the network. Please always consider the followin
1. byte order - in what order of bytes the integer number is stored in the memory. When sending the number from one order to another without a proper conversion it changes its value (e.g. 0x00FF to 0xFF00). Fortunately you do not need to know your local byte order, instead you always convert all the integers to so called network byte order with the macros htons (1.  bit shorts) and htonl (32 bit integers) before sending them and again convert it from this order back to host order with the macros ntohs and ntohl just after receiving them.
1. There is no network byte order (or a format) for floating point numbers. As their format varies on different architectures you must send them as text or as fixed precision numbers (integers).
1. The one byte character string is not affected by the byte order, in most cases sending the data in textual (human readable) format is a good and safe choice. All one byte data types are byte order safe.
1. Integer types in C language do not have standardized  size. They can have different size depending on the architecture or even the compiler. To avoid the problems it is safer to use size defined types like int32_t or uint1. _t.
1. Sending a structure over the network can cause problems too. It is related to the way compilers layout the structure fields in the memory. On some architectures it is faster to read the memory bytes at addresses divisible by 8 on other by 1. ,32 or other divider. Compiler tries to speed up the access by placing all the fields on fast addresses and it means gaps of unknown size between the members. If those gaps differ then after binary transfer the structure will not be usable. To avoid this, you must be able to tell the compiler to "pack" the structure - remove the gaps, but it can not be achieved in a portable way within the code. To avoid the problem you can send the structure member by member.

## Task on local + TCP sockets
Write simple integer calculator server. Data send to server consists of:
- operand 1
- operand 2
- result
- operator (+,-,*,/)
- status

all converted to 32 bit integers in an array.

Server calculates the results of operation (+,-,*,/) on operands and sends
the result back to the client. If operation is possible status returned is 1. 
otherwise it should be 0. Server must work with 2 types of connection:
- local stream sockets
- network tcp sockets

Server is single process application, it takes 2 parameters:
- local socket file name
- port
	
Write 2 types of client, one for each connection type, those clients shall take the following parameters:
- address of the host (file name for local connection, domain name for inet)
- port number (tcp client only)
- operand 1
- operand 2
- operator (+,-,*,/)

On success client displays the result on the screen.
All above programs can be interrupted with C-c, server may NOT leave local socket file not deleted in such a case.
		
### Solution

What you must know:

```
man 7 socket
man 7 epoll
man 7 unix
man 7 tcp
man 3p socket
man 3p bind
man 3p listen
man 3p connect
man 3p accept
man 2 epoll_create
man 2 epoll_ctl
man 2 epoll_wait
man 3p freeaddrinfo (obie funkcje, getaddrinfo też)
man 3p gai_strerror
```

Pay closer attention at Q&A in `man 7 epoll`. It is well prepared so we will not repeat it here.


Common library for all sources in this tutorial:
{{< includecode "l7_common.h" >}}

server `l7-1_server.c`:
{{< includecode "l7-1_server.c" >}}
local client `l7-1_client_local.c`:
{{< includecode "l7-1_client_local.c" >}}
TCP client `l7-1_client_tcp.c`:
{{< includecode "l7-1_client_tcp.c" >}}

To run the code:
```
$ ./l7-1_server a 2000&
$ ./l7-1_client_local a 2 1 +
$ ./l7-1_client_local a 2 1 '*'
$ ./l7-1_client_local a 2 0 /
$ ./l7-1_client_tcp localhost 2000 234 17  /

$ killall -s `SIGINT` prog23a_s

```

In this solution (and also in the next example) all sources uses common library to avoid implementing functions like `bulk_read` many times.

You may be curious why the constant BACKLOG is set for 3, why not 5,7 or 9? In practice  any small number  would fit here, this value is merely a hint for the operating system. This program will not deal with a large amount of network traffic and it handles connections very promptly so the queue of waiting connections will never be long. When you expect a heavier traffic in your program please test larger values and find the smallest one that suits your needs. Unfortunately this value will be different on different operating systems.

In this code macro SUN_LEN is used. Why not to use sizeof instead? Both approaches work correctly. You should know, that the sizeof will return slightly larger size than the macro due to the count method. Unlike sizeof, the macro does not count the gap between the members of the address structure. The implementation expects the smaller value of those two but as the address is just a zero delimited string the larger size value has no effect on the address itself. What should you choose? In this tutorial sizes are calculated with the macro SUN_LEN as the standard demands. In this way we save a few bytes of the memory reserved for the address at the cost of a few CPU cycles more to count the size properly. If you decide that CPU has more priority over memory you can choose to use sizeof it will not be considered as an error.

POSIX standard states that if network connecting gets interrupted it must continue asynchronously to the main code. It is quite logical if you remember that connecting engages two processes usually on different computers. If signal handling function interrupts the connect you can not restart as usual (using `TEMP_FAILURE_RETRY`), it will result in EALREADY error. In our client code we do not handle signals, but how it can be done? You need to check if `connect` was interrupted (`errno==EINTR`) and then use `select`, `poll`, or `epoll*` until socket will be ready to write.

As we always have to  implement asynchronous waiting for connect, it may be reasonable not to waste the time on the connection and plan some code to run in the meantime, we can set the socket into nonblocking and have the connecting done in the background even if connect is not interrupted.

This program implements a trivial network data exchange schema (so called protocol). Client connects and sends the request, server responds and both parties disconnect. Much more complex protocols are possible in your programs of course.

All data exchanged  up to given point of connection time creates what we refer to as a context of the transmission. Depending on the connection protocol context can be more or less complex. In this program the only context that exists is the question sent by the client.  The response is based on the context (the question).

You may ask why macros ntohl and htonl are in use as the connection is local and byte order should not be a concern? First of all, the same code will be reused in the second stage for TCP network connection that demands the conversion. The second reason is that local sockets are not limited to work only on local file systems. In future, it may be possible to create such a socket on a network file system and connect two different machines/architectures. Then the byte order would be an issue that this code can handle.

In this code, function bulk_read is used, it is important to know how this function will work in case of nonblocking descriptor. If data is not available it will return immediately with EAGAIN error. Is this the case in this code? Is the descriptor in not blocking mode? Newly created descriptor returned by accept function does not have to inherit the flags! In case of Linux flags are indeed not inherited so as far as Linux is used not blocking mode of the descriptor does not interfere with be bulk_read. In fact it wold not cause problems also on the platforms that inherit the  not blocking flag due to the fact that we call bulk_read after we learn that the data is already available.

You are obliged to use getaddrinfo function, the older gethostbyname function is described as obsolete in the man page and can not be used in your solutions.

How can you check on the socket file after you started the server?
{{< answer >}} $ls -l a  {{< /answer >}}

What is the role of `epoll_pwait` call in this program?
{{< answer >}} This is the point the program waits for data input from the descriptors and at the same time waits for SIGINT signal delivery {{< /answer >}}

Can we replace `epoll_pwait` with `epoll_wait`?
{{< answer >}} Yes, but it is not worth the effort as then proper SIGINT handling would be much more code demanding {{< /answer >}}

Why network listening socket is in non blocking mode? 
{{< answer >}} With this descriptor in blocking mode it is possible to block the program on "accept" call. Consider the scenario of the client trying to connect and then unexpectedly vanishing from the network. It may happen that `epoll_pwait` will report the socket to be ready for a connection but after the call the new client will disappear and at the time of accept call there will be no one to connect. The program will simply block on the accept until another client arrives. The not blocking mode prevents this situation. {{< /answer >}}

Why program uses int32_t (stdint.h) instead of plain int? 
{{< answer >}} Due to various sizes of the int on different architecture. {{< /answer >}}

Why SIGPIPE is ignored in the server?
{{< answer >}} It is generally easier to handle the broken pipe condition by checking for EPIPE error rather than handling the SIGPIPE signal when the reaction tho the disconnection is not critical - server only closes the current connection and continues to serve other clients. {{< /answer >}}

Why bulk_read nad bulk_write are used in the program? SIGINT is terminating the program thus interruption of read and write should not be a problem.
{{< answer >}} For the same reason TEMP_FAILURE_RETRY is so common in the code - portability of this code to your solution, with bulk_read/write the code is interruption proof. {{< /answer >}}

What is the purpose of the unlink in the server code?
{{< answer >}} It removes the local socket the same way it removes a file or a fifo - clean up function. {{< /answer >}}

What is the purpose of socket option SO_REUSEADDR? 
{{< answer >}} This option allows you to quickly bind to the same port on the server if the program run a moment ago. It is essential for testing when you encounter a bug, quickly correct it and then want to run the program again. Without this option system will block binding to the port for a few minutes. {{< /answer >}}

Does the above socket option mean that the packets from previous connection can still be received in a new session?
{{< answer >}} No, TCP protocol is immune to this kind of distraction, if it was UDP then the answer would be yes. {{< /answer >}}

Why the address INADDR_ANY is used for the server, what is the value of this constant?</br>
{{< answer >}} This is a special address, the value is  0.0.0.0. It means any address. It is used as local address of the server in place of  real IP of the server (server can have more that one IP). It does not mean the program will be caching all the Internet connections! If the connection is directed (routed) to our server then we do not care what IP address client used, if ports match then connection is handled without IP matching. Very popular and convenient solution. {{< /answer >}}

TCP client code and local client code are very similar, as a exercise integrate those two types of client into one program with parameter swich (-p local|tcp)

## Sample task

Complete the sample exercises. You will have more time and starter code during the lab session, but completing the tasks below on your own means you are well prepared.

- [Exercise 1]({{< ref "/sop2/lab/l7/example1" >}}) ~60 minutes
- [Exercise 2]({{< ref "/sop2/lab/l7/example2" >}}) ~120 minutes (stages 4-5 are for L8)
- [Exercise 3]({{< ref "/sop2/lab/l7/example3" >}}) ~120 minutes
- [Exercise 4]({{< ref "/sop2/lab/l7/example4" >}}) ~120 minutes

## Source codes presented in this tutorial

{{% codeattachments %}}

## Other references

 - <http://cs341.cs.illinois.edu/coursebook/Networking#layer-4-tcp-and-client>
 - <http://cs341.cs.illinois.edu/coursebook/Networking#layer-4-tcp-server>
 - <http://cs341.cs.illinois.edu/coursebook/Networking#non-blocking-io>
