---
title: "L8 - UDP and ?"
weight: 50
---

## WIP

## UDP

Goal:
 
Write client and a server program that communicate over UDP socket. Client task is to send a file divided into proper size datagrams to the server. Server prints out the received data without information about the source.
Each packet send to server must be confirmed with return message. If confirmation is missing (wait 0,5 sec.) resend the packet again. If 5 tries in a row fail, client program exits with an error. Both data packets and confirmations can be lost, program must resolve this issue. Server can not print the same part of the file more than once.
All metadata (everything apart from file content) send over the udp socket must be converted to int32_t type. You can assume that maximum allowed datagram (all data and metadata) size is 576B. Server can handle 5 concurrent transmissions at a time. If sixth client tries to send data it should be ignored.
Server program takes port as its sole parameter, client takes address and port of the server as well as file name as its parameters. 
	

What you need to know:
```
man 7 udp
man 3p sendto
man 3p recvfrom
man 3p recv
man 3p send
```


Solution `l8-1_server.c`:
{{< includecode "l8-1_server.c" >}}

Solution `l8-1_client.c`:
{{< includecode "l8-1_client.c" >}}


There is no connection in UDP protocol, sockets send datagrams "ad hoc". There is no listening socket. Losses, duplicates and reordering of datagrams  are possible!

In this example you will find useful library candidate functions like: make_socket, bind_inet_socket as they have conflicting names with previously recommended functions, you have to rename them.

In this example connection context is more demanding. What constitutes the context here?
{{< answer >}} The number of packets send of specified file  up to given moment is the context here - in other words struct connections. {{< /answer >}}

What data is sent in datagram? What is the purpose of the metadata?</br>
{{< answer >}} The datagram consists of (1.  32 bit file part number, (2) 32 bit information if this is the last part of the file, (3) the file part.  Metadata helps to maintain the context, keep the track of the file being sent (1.  and end the transmission (2).   {{< /answer >}}

Why and on what descriptors bulk_read and bulk_write are used? Should we extend this use on all the descriptors in the program?</br>
{{< answer >}} Mentioned functions are used to restart read and write after interruption on IO. Notice that it is restarting both interruption types: before IO starts (EINTR) and  the interruption in the middle of IO. Those functions are only used on files as datagrams are sent in atomic way (transfer can not be interrupted). In this program signals are handled in code parts that do not operate on files, still due to code portability bulk_ functions are used. {{< /answer >}}

Do we expect broken connection in this program? Should we add checks in the code?
{{< answer >}} We do not have connection to break in UDP. {{< /answer >}}

How findIndex works in server code: How addresses are compared? What byte order they are in? What will the function do if the address in new?
{{< answer >}} Addresses are compared in binary format without conversion to host byte order. We do not need to convert the byte order as we only compare the addresses, we do not display them. If new address is passed to this function it starts a new record for this address in the array (provide there is a space for it). {{< /answer >}}

How this program deals with the duplicates of datagrams?
{{< answer >}} It keeps an array of active connections "struct connections"  with the number of last transferred part. Duplicated parts are not processed. {{< /answer >}}

How this program deals with the reordered datagrams, i.e. when program receives a part that is farther in the file than the next expected?
{{< answer >}} This can not happen in this program, client will not send such a part without having all the previous parts confirmed by the server. {{< /answer >}}

How this program handles lost datagrams?
{{< answer >}} Client side re-transmission. {{< /answer >}}

What will happen if the packet with confirmation from the server to the client gets lost?
{{< answer >}} The client will assume that the last part sent did not get to the server, it will send it again. Server will get the duplicate of the part that is already stored, it will not process it but it will send another confirmation.  {{< /answer >}}

What is in the confirmation?
{{< answer >}} Server sends back exactly the same data as it received. {{< /answer >}}

How timeout on the server response is implemented? 
{{< answer >}} In function sendAndConfirm 0.5s alarm is set (with setitimer) then the program waits for the confirmation on regular not restarted recv call. If signal is received, the recv gets interrupted and the code checks if the timeout triggered. {{< /answer >}}

Why the program converts byte order only the part number and the last part marker and not the rest?
{{< answer >}} Only those two are send in binary integer format, the rest is a file part send as text string that does not require the conversion. {{< /answer >}}

Analyze how 5 connection limit works, pay attention how "free" member in the connections structure works, how it is affected by the last part marker in the datagram?

## Sample task

Complete the sample exercises. You will have more time and starter code during the lab session, but completing the tasks below on your own means you are well prepared.


## Source codes presented in this tutorial

{{% codeattachments %}}

## Other references

 - <https://cs341.cs.illinois.edu/coursebook/Networking#layer-4-udp>
